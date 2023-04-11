/* This is simple demonstration of how to use expat. This program
   reads an XML document from standard input and writes a line with
   the name of each element to standard output indenting child
   elements by one tab stop more than their parent element.

   This version uses wchar_t (UTF-16) instead of char (UTF-8).
*/

#define XML_UNICODE_WCHAR_T 1

#include <stdio.h>
#include <expat.h>

#ifdef XML_LARGE_SIZE
#if defined(XML_USE_MSC_EXTENSIONS) && _MSC_VER < 1400
#define XML_FMT_INT_MOD "I64"
#else
#define XML_FMT_INT_MOD "ll"
#endif
#else
#define XML_FMT_INT_MOD "l"
#endif

static void XMLCALL
startElement(void *userData, const wchar_t *name, const wchar_t **atts)
{
  int i;
  int *depthPtr = (int *)userData;
  for (i = 0; i < *depthPtr; i++)
    putchar('\t');
  _putws(name);
  *depthPtr += 1;
}

static void XMLCALL
endElement(void *userData, const wchar_t *name)
{
  int *depthPtr = (int *)userData;
  *depthPtr -= 1;
}

#ifdef AMIGA_SHARED_LIB
#include <proto/expat.h>
int
amiga_main(int argc, char *argv[])
#else
int
main(int argc, char *argv[])
#endif
{
  char buf[BUFSIZ];
  XML_Parser parser = XML_ParserCreate(NULL);
  int done;
  int depth = 0;
  XML_SetUserData(parser, &depth);
  XML_SetElementHandler(parser, startElement, endElement);
  do {
    size_t len = fread(buf, 1, sizeof(buf), stdin);
    done = len < sizeof(buf);
    if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
      fprintf(stderr,
              "%s at line %" XML_FMT_INT_MOD "u\n",
              XML_ErrorString(XML_GetErrorCode(parser)),
              XML_GetCurrentLineNumber(parser));
      return 1;
    }
  } while (!done);
  XML_ParserFree(parser);
  return 0;
}
