#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <string.h>
#include <conio.h>
#include <time.h>


typedef unsigned int  UINT;
typedef unsigned long ULONG;
typedef          char CHAR;
typedef unsigned char BOOL;
typedef double        REAL;

typedef struct node {
   UINT idx;               /* ID code for attribute */
   REAL threshold;         /* Numerical threshold for attribute test */
   struct node *on;        /* Address of 'on' node */
   struct node *off;       /* Address of 'off' node */
   struct node *parent;    /* Addess of parent node */
} NODE;
typedef struct ne_struct {
    REAL ne;
    UINT status;
} NEGENTROPY;

typedef struct matrix {
   UINT width;
   UINT height;
   REAL **data;
} MATRIX;

enum UINT { INACTIVE, OFF, ON };
#define LN_2 0.693147180559945309417
#define entropy(x) (x > 0 ? x * log(x) / LN_2 : 0.0)




NEGENTROPY negentropy ( REAL **, UINT, NODE*, UINT );
void print_tree ( NODE* , CHAR** );
void free_tree ( NODE  * );
NODE* ID3 ( MATRIX * , NODE* , UINT , UINT );
void err_exit ( CHAR* , UINT );
MATRIX *build_matrix ( UINT, UINT );
void free_matrix ( MATRIX * );
void read_matrix ( CHAR *, MATRIX * );
void file_size ( CHAR * , UINT * , UINT * );
CHAR **read_tags ( CHAR * , UINT );
void free_tags ( CHAR **, UINT);



/*-------------------------------------------------------------------*/

MATRIX *build_matrix (UINT width, UINT height)
{
    MATRIX *_matrix;
    UINT i;

    _matrix = (MATRIX*) malloc (sizeof (MATRIX));
    if (!_matrix)
        err_exit (__FILE__, __LINE__);

    _matrix->width  = width;
    _matrix->height = height;

    _matrix->data = (REAL**) malloc (height * sizeof (REAL*));
    if (_matrix->data == NULL)
        err_exit(__FILE__, __LINE__);

    for (i=0; i<height; i++)
    {
        _matrix->data[i] = (REAL*) malloc (width * sizeof(REAL));
        if (_matrix->data[i] == NULL)
            err_exit(__FILE__, __LINE__);
    }
    return _matrix;
}

/*-------------------------------------------------------------------*/

/*
* Standard error handler function
*/

void err_exit (CHAR* file, UINT line)
{
    printf("\n Fatal error in file %s, line %u", file, line);
    exit(0);
}

/*-------------------------------------------------------------------*/

void file_size (CHAR *file_name, UINT *width, UINT *height)
/*
* Given the name of a file of numeric data, this routine counts
* the numbers of rows and columns. It's assumed that the number
* of entries is the same in each row, and an error is flagged if this
* is not the case.
*
*/
{
    FILE *f;
    UINT buf_size = 0xFF, _width = 0;
    CHAR *buffer, *ptr;

    *width = *height = 0;

    buffer = (CHAR*) malloc (buf_size * sizeof (CHAR));
    if (buffer == NULL)
        err_exit (__FILE__, __LINE__);

    /* Open price file - abort if filename invalid */
    f = fopen(file_name, "r");
    if (f == NULL)
    {
        printf("\n File not found : %s\n", file_name);
        err_exit (__FILE__, __LINE__);
    }

    /* Get number of entries in first row */
    if (fgets(buffer, buf_size, f) != NULL)
    {
        ++*height;
        ptr = strtok (buffer, " ,");
        while (ptr != NULL)
        {
            ++*width;
            ptr = strtok (NULL, " ,");
        }
    }

    /* Count numbers of subsequent rows */
    while (!feof(f))
    {
        if (fgets(buffer, buf_size, f) != NULL)
        {
            if (strlen(buffer) > strlen("\n"))  /* if line is more than a NL char */
            {
                ++*height;
                _width = 0;
                ptr = strtok (buffer, " ,");
                while (ptr != NULL)
                {
                    ++_width;
                    ptr = strtok (NULL, " ,");
                }

                if (*width != _width)
                {
                    printf("\n Number of entries in file %s did not agree", file_name);
                    err_exit (__FILE__, __LINE__);
                }
            }
        }
    }
    free (buffer);
}

/*-------------------------------------------------------------------*/

void free_matrix (MATRIX *_matrix)
{
    UINT i;
    for (i=0; i<_matrix->height; i++)
        free (_matrix->data[i]);

    free (_matrix->data);
    free (_matrix);
}

/*-------------------------------------------------------------------*/

void free_tags ( CHAR** varname, UINT width)
{
    UINT i;
    for (i=0; i<width; i++)
        free(varname[i]);
    free (varname);
}

/*-------------------------------------------------------------------*/

void free_tree ( NODE  *node )
{
    /*
     *  Frees the memory allocated to a tree structure
     */

    if (node == NULL)
        return;
    else
    {
        free_tree (node->on);
        free_tree (node->off);
        free(node);
    }

}

/*-------------------------------------------------------------------*/

NODE* ID3 ( MATRIX *matrix, NODE* parent, UINT target, UINT state)

/* Routine to build a decision tree, based on Quinlan's ID3 algorithm. */
{
    NEGENTROPY negentropy_struct;
    NODE *node;
    UINT n_vars = matrix->width, n_samples = matrix->height, i, j, split;
    REAL **data = matrix->data;
    REAL best_threshold, min_negentropy, _negentropy;

    /* Allocate memory for this node */
    node = (NODE*) malloc (sizeof(NODE));
    if (!node)
        err_exit (__FILE__, __LINE__);

    /* Set up links in decision tree */
    node->parent = parent;  /* Set address of parent node */

    if (parent != NULL) /* parent to child; not relevant for root node */
    {
        /* Pass address of this node to the parent node */
        if (state == ON)
            parent->on = node;
        else
            if (state == OFF)
                parent->off = node;
    }

    /*
     * Select attribute with lowest negentropy for splitting. Scan through
     * ALL attributes (except the target) and ALL data samples. This is
     * pretty inefficient for data sets with repeated values, but will do
     * for illustrative purposes
     */

    min_negentropy = 1.0;

    for (i=0; i<n_vars; i++)
    {

        for (j=0; j<n_samples; j++) 
        {

            if (i != target) 
            {

                /* Set trial values for this node... */
                node->idx = i;
                node->threshold = data[j][i];

                /* ...and calculate the negentropy of this partition */
                negentropy_struct = negentropy (data, n_samples, node, target);

                _negentropy = negentropy_struct.ne;

                /* If this negentropy is lower than any other, retain the
                       index and threshold for future use */
                if (_negentropy < min_negentropy) 
                {
                    min_negentropy = _negentropy;
                    split = i;
                    best_threshold = data[j][i];
                }

            } /*if (i != target)*/

        } /*for (j=0; j<n_samples; j++)*/

    } /*for (i=0; i<n_vars; i++)*/

    /* Save the combination of best attribute and threshold value */
    node->idx = split;
    node->threshold = best_threshold;

    /*
     * If the negentropy routine found itself at an end-of-branch
     * for the decision tree, the 'status' flag in 'negentropy_struct'
     * is set to ON or OFF and the node labelled accordingly. Otherwise,
     * ID3 continues to call itself until all end-of-branch nodes are
     * found.
     */

    if  (negentropy_struct.status != INACTIVE) 
    {
        node->on = node->off = NULL;
        node->idx = negentropy_struct.status;
    }
    else
    {
        node->on  = ID3 (matrix, node, target, ON);
        node->off = ID3 (matrix, node, target, OFF);
    }

    return node;
}

/*-------------------------------------------------------------------*/

void main (int argv, char *argc[])
{

    MATRIX *matrix;
    NODE *node;
    UINT target, n_vars, n_samples;
    CHAR data_file[13], tag_file[13];  /* Longest file name in DOS */
    CHAR **tag_names;

    /* Set up file names */
    if (argv != 2)
    {
        printf("\nUsage: id3 [datafile]");
        exit(0);
    }
    else
    {
        printf("\nWelcome to ID3");
        printf("\nLast compiled on %s, %s", __DATE__, __TIME__);
        printf("\n");
        strcpy(data_file, argc[1]);
        strcpy(tag_file,  argc[1]);
        strcat(data_file, ".dat");
        strcat(tag_file,  ".tag");
    }

    /* Read dimensions of data file */
    file_size (data_file, &n_vars, &n_samples);

    /* Read labels for columns of data */
    tag_names = read_tags (tag_file, n_vars);

    /* Allocate storage for data... */
    matrix = build_matrix (n_vars, n_samples);

    /* ...and read it from disk */
    read_matrix (data_file, matrix);

    /* Classification target is last column */
    target = n_vars - 1;

    /* Return root of decision tree - ID3 continues to call itself
       recursively */

    node = ID3 ( matrix, NULL, target, 0 );

    print_tree(node, tag_names);

    printf("\n");

    free_tags (tag_names, n_vars);
    free_matrix(matrix);
    free_tree (node);
}

/*-------------------------------------------------------------------*/

NEGENTROPY negentropy ( REAL **data,
    UINT   n_samples,
    NODE   *local,
    UINT   target)
{
    /*
     * Calculates the entropy of classification of an attribute, given
     * a table of attributes already used, the attribute on which splitting
     * is to be taken, and the target attribute. Entropy is calculated in
     * bits, so logs are taken to base 2 by dividing by LN_2.
     *
     * The returned value always lies in the (closed) range [0, 1].
     */

    NEGENTROPY ret_val;
    NODE *_node, *_parent;
    UINT on_ctr, off_ctr, p1, p2, i, _match;
    REAL p_on, p_off, negentropy_on, negentropy_off;

    on_ctr = off_ctr = p1 = p2 = 0;

    /* Scan through all supplied data samples */
    for (i=0; i<n_samples; i++) 
    {

        /*
         * If pattern matches the current position in the decision tree,
         * then use this vector. The match is determined by passing up
         * the decision tree and checking whether 'data[idx] >= threshold'
         * matches at each step, where idx and threshold are taken from
         * each node in turn.
         */

        _match = 1;
        _node = local;
        while (_node->parent != NULL) 
        { /* If at the root node, all entries match*/
            _parent = _node->parent;
            if (_node == _parent->on) 
            { /* if parent node is ON */
                if (data[i][_parent->idx] < _parent->threshold)
                    _match = 0;
            }
            else
                if (_node == _parent->off) 
                { /* if parent node is OFF */
                    if (data[i][_parent->idx] >= _parent->threshold)
                        _match = 0;
                }
            _node = _parent;
        }

        if (_match) 
        {
            if (data[i][local->idx] >= local->threshold) 
            {
                on_ctr++;
                if (data[i][target] >= 0.5)
                    p1++;
            }
            else 
            {
                off_ctr++;
                if (data[i][target] >= 0.5)
                    p2++;
            }
        }
    }   /* for (i=0; i<n_samples; i++) */

    /* 1: Entropy of subtable with activation ON */

    /*
     * We now have the numbers of samples that match this part of the
     * decision tree, and the number of samples for which the supplied
     * condition are true. From these quantities we can find the negentropy of
     * this partition.
     */

    if (on_ctr > 0)
    {
        p_on  = (REAL) p1 / (REAL) on_ctr;
        p_off = 1 - p_on;
        negentropy_on = -entropy (p_on) - entropy (p_off);
    }
    else
        negentropy_on = 0.0;

    /* 2: Entropy of subtable with activation OFF */

    if (off_ctr > 0)
    {
        p_on  = (REAL) p2 / (REAL) off_ctr;
        p_off = 1 - p_on;
        negentropy_off = -entropy (p_on) - entropy (p_off);
    }
    else
        negentropy_off = 0.0;

    ret_val.ne = (negentropy_on * on_ctr + negentropy_off * off_ctr);
    ret_val.ne /= (on_ctr + off_ctr);

    /*
     * If all values examined were the same, set 'ret_val.status' to
     * the target value since this will be an end-of-branch node
     */

    if ((p1 == on_ctr) && (p2 == off_ctr))
        ret_val.status = ON;
    else if ((p1 == 0) && (p2 == 0))
        ret_val.status = OFF;
    else
        ret_val.status = INACTIVE;

    return ret_val;

}

/*-------------------------------------------------------------------*/

void print_tree (NODE *node, CHAR** tag_names)
{
    /*
     * Displays algebraic equivalent of the decision tree
     */

    if (node->on == NULL) 
    {
        if (node->idx == ON)
            printf("ON");
        else if (node->idx == OFF)
            printf("OFF");
        return;
    }

    else
        printf("if { %s >= %1.2f then ", tag_names[node->idx], node->threshold);

    print_tree ( node->on, tag_names );
    printf(" else ");
    print_tree ( node->off, tag_names );
    printf( " } " );
}

/*-------------------------------------------------------------------*/

void read_matrix (CHAR filename[], MATRIX *matrix)

{

    UINT i, j;
    UINT height = matrix->height;
    UINT width  = matrix->width;
    REAL **data = matrix->data;
    FILE *f;

    /* Open price file */
    if ((f = fopen(filename, "r")) == NULL) 
    {
        printf("\n File not found : %s\n", filename);
        err_exit (__FILE__, __LINE__);
    }

    for (i=0; i<height; i++)
        for (j=0; j<width; j++)
        {
            fscanf(f, "%lf\n", &data[i][j] );
        }

    fclose(f);

}

/*-------------------------------------------------------------------*/

CHAR **read_tags (CHAR *tag_file, UINT width)
{
    FILE *f;
    CHAR **_varname;
    UINT i;
    CHAR buffer[0xFF];

    f = fopen(tag_file, "r");
    if (f == NULL)
    {
        printf("\n File not found : %s\n", tag_file);
        err_exit (__FILE__, __LINE__);
    }

    _varname = (CHAR**) malloc (width * sizeof (CHAR*));
    for (i=0; i<width; i++)
        _varname[i] = (CHAR*) malloc (0xFF * sizeof (CHAR));

    i = 0;
    while (!feof(f))
    {
        if (fgets(buffer, 0xFF, f) != NULL)
        {
            if (strlen(buffer) > strlen("\n"))
            {
                if (i>width-1)
                {
                    printf("\nMore variable names were detected than data items.");
                    printf("\nPlease correct this problem before proceeding");
                    exit(0);
                }
                sscanf (buffer, "%[a-zA-Z0-9-_;:!@#$%^&*(){}[]]", _varname[i]);
                i++;
            }
        }
    }

    if (i<width) 
    {
        printf("\nFewer variable names than data items were detected.");
        printf("\nPlease correct this problem before proceeding");
        exit(0);
    }

    fclose (f);

    return _varname;

}