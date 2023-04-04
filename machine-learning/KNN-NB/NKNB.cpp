#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<string>
#include<set>
#include<vector>
#include<algorithm>
#include<cmath>
#include<cstring>
#include<cstdlib>

using namespace std;

const int TEST_SIZE = 5;
const int TRAINED_SIZE = 5;
const float LP_BASE = 0.01;

//global definations
set<string> keySet;
string em_list[20][20];

float dataVec[20][100]={{0}};
int vec_position[20][20]={{0}};
int vec_len[20]={0};

//Get NB rate
void GetNBRate(float rate[],int testLength,int trainLength);
//Get the Euler diatance
float getEulerDistance(float vec1[],float vec2[],int length);
//Get the City block distance
float getCityblockdistance(float vec1[],float vec2[],int length);
//Get the Supremum distance
float getSupremumdistance(float vec1[],float vec2[],int length);
// Read the dataset words
void readKeySet(char* filename);
//Read the rate data of the trained files
void readDataFrom(const char* filename,float*);
//Write the rate data to test files
void WriteTo(const char* filename,float vec[]);
//To vectorize the data Set
void Vectorize(int testLength,int trainLength);
//Get the rate of test files
void getKNN1Rate(float rate[],int );
//Get IDW Rate
void getIDWRate(float rate[],int ,int );
//Get KNN Rate
void getKNNRate(float rate[],int length,int N);
//Get Solution
void GetSolution();
//Get the n Minimal Indexes of array a
int* getMinIndexes(float a[],int len,int n);

int main()
{
    GetSolution();
    
	return 0;
}

void GetSolution()
{
    //freopen("t.txt","w",stdout);
    float rate[20];
    int vec_length = 0;

    // Read the dataset words
    readKeySet((char*)"Dataset_none.txt");
    vec_length = keySet.size();
    
    //Read the rate data of the trained files
    //for(int i=0;i<6;i++)
    readDataFrom((char*)"rate.txt",rate);

    //To vectorize the data Set
    Vectorize(TEST_SIZE,TRAINED_SIZE);
    
    ofstream out("DataVector.txt");
	for(int i=0;i<TEST_SIZE+TRAINED_SIZE;i++)
    {
    	out<<"t"<<i;
    	for(int j=0;j<vec_length;j++)
	    {
	 		out<<" "<<dataVec[i][j];
	    }
	    out<<endl;
    }
    out.close();
    
    //Get the rate of test files
    getKNN1Rate(rate,vec_length);

    //Get IDW Rate
    //getIDWRate(rate,vec_length,13);

    //Get NB rate
    //GetNBRate(rate,TEST_SIZE,TRAINED_SIZE);

    //Write the rate data to test files
	//for(int i=0;i<6;i++)
    WriteTo((char*)"predict.txt",rate);
}

void getIDWRate(float rate[] ,int length,int N)
{
    cout<<"Get IDW Rate:";
    float temp_r = 0,temp_d[50],all_a,temp_a,BIG_D=10;
    int Update_N;

    for(int i=TRAINED_SIZE;i<TEST_SIZE+TRAINED_SIZE;i++)
    {
        Update_N = N-1;
        for(int j=0;j<TRAINED_SIZE;j++)
        {
            temp_d[j] = getCityblockdistance(dataVec[i],dataVec[j],length);//
        }

        //int* indexes = new int[N];
        int* indexes = getMinIndexes(temp_d,TRAINED_SIZE,N);

        //cout<<indexes[0]<<endl;
        //delete [] indexes;
        
        //while(temp_d[indexes[Update_N]]-temp_d[indexes[0]] > (float)1e-2) Update_N--; //¼«²î<1e-2
        //while(temp_d[indexes[Update_N]]-temp_d[indexes[0]] > (float)1e-2)  temp_d[indexes[Update_N--]] = BIG_D;
        
        temp_r = 0;
        all_a = 0;
        for(int p=0;p<=Update_N;p++)
        {
            temp_a = pow((1.0/temp_d[indexes[p]]),13); //temporary value led result to up to 0.15
            //*(1.0/temp_d[indexes[p]])*(1.0/temp_d[indexes[p]]);

            temp_r += rate[indexes[p]]*temp_a;
            all_a += temp_a;
        }
        rate[i]= temp_r/all_a;
    }
    cout<<" Done!"<<endl;
}


int* getMinIndexes(float a[],int len,int n)
{
    int index,time,totaltimes=0;
    float min,current_min=-1;
    int* out = new int[n];

    for(int i=0;i<n;i++)
    {
        min=0xFFFFF;
        time = 0;
        index = 0;

        for(int j=0;j<len;j++)
        {
            if(min>a[j])
            {
                if(time<=totaltimes&&a[j]==current_min)
                {
                    time++;
                }else if(a[j]>=current_min)
                {
                    min = a[j];
                    index = j;
                }

            }
        }

        if(min == current_min)
            totaltimes++;
        else
        {
            totaltimes = 0;
            current_min = min;
        }

        out[i] = index;

    }

    return out;
}


void getKNNRate(float rate[] ,int length,int N)
{
    cout<<"Get "<<N<<"-NN Rate:";
    float temp_r = 0,temp_d[50];
    
    for(int i=TRAINED_SIZE;i<TEST_SIZE+TRAINED_SIZE;i++)
    {
        for(int j=0;j<TRAINED_SIZE;j++)
        {
            temp_d[j] = getEulerDistance(dataVec[i],dataVec[j],length);
        }
        
        //int* indexes = new int[N];
        int* indexes = getMinIndexes(temp_d,TRAINED_SIZE,N);
        
        //cout<<indexes[0]<<endl;
        //delete [] indexes;
        
        temp_r = 0;
        for(int p=0;p<N;p++)
        {
            temp_r += rate[indexes[p]];
        }
        rate[i]= temp_r/(float)N;
    }
    cout<<" Done!"<<endl;
}

void GetNBRate(float rate[],int testLength,int trainLength)
{
    int current_len =0,position = 0;
    float sum_rate = 0,sum=0,temp;  // 6 emotions
    for(int i=trainLength;i<testLength+trainLength;i++)
    {
        current_len= vec_len[i];
        
        sum_rate=0;
        for(int j=0;j<trainLength;j++)
        {
            sum = 0;
            //sum = rate[j];
            for(int l=0;l<current_len;l++)
            {
                position = vec_position[i][l];
                
                sum += dataVec[j][position];
//                if(dataVec[j][position]>LP_BASE)
//                    sum *= dataVec[j][position];
//                else
//                    sum *= LP_BASE;
//            }
            
	            sum_rate += rate[j]*sum;
	            //sum_rate += sum;
            }
        
        	rate[i]= sum_rate>1?1:sum_rate;
        }
    }
    
    cout<<"Get NB Rate: Done!"<<endl;
}

void getKNN1Rate(float rate[],int length){
    int index = 0;
    float mindis = (float)0xffffff;
    for(int i=TRAINED_SIZE;i<TEST_SIZE+TRAINED_SIZE;i++)
    {
        index = 0;
        mindis = (float)0xffffff;
        
        for(int j=0;j<TRAINED_SIZE;j++)
        {
            float temp = getEulerDistance(dataVec[i],dataVec[j],length);
            if (mindis > temp)
            {
                mindis = temp;
                index = j;
            }
        }
        rate[i]= rate[index];
    }
    cout<<"GetRate: Done!"<<endl;
}

void Vectorize(int testLength,int trainLength){
    int position = 0,str_len=0,jj=0;
    set<string>::iterator begin = keySet.begin();
    set<string>::iterator it;
    
    for(int i=0;i<testLength+trainLength;i++)
    {
        position = 0;
        //str_len= sizeof(em_list[i]) / sizeof(em_list[i][0]); //wrong
        str_len = vec_len[i];
        
        for(int j=0;j<str_len;j++)
        {
            it = keySet.find(em_list[i][j]);
            position = std::distance(begin,it);
            
            //dataVec[i][position]=1.0/(float)str_len;
            dataVec[i][position]=1;
            
            vec_position[i][j] = position; //store position here for later calculations
        }
    }
    
    cout<<"Vectorization: Done!"<<endl;
}


float getEulerDistance(float vec1[],float vec2[],int length){
    if(length == 0) return 0;
    float sum = 0;
    
    for(int i=0;i<length;i++)
    {
        sum+=(vec1[i]-vec2[i])*(vec1[i]-vec2[i]);
    }
    return sqrt(sum);
}

//Get the City block distance
float getCityblockdistance(float vec1[],float vec2[],int length)
{
    if(length == 0) return 0;
    float sum = 0;
    
    for(int i=0;i<length;i++)
    {
        sum+=abs(vec1[i]-vec2[i]);
    }
    return sum;
}

//Get the Supremum distance
float getSupremumdistance(float vec1[],float vec2[],int length)
{
    if(length == 0) return 0;
    float max = 0,temp;
    
    for(int i=0;i<length;i++)
    {
        temp = abs(vec1[i]-vec2[i]);
        if (temp > max)
            max = temp;
    }
    return max;
}

void readDataFrom(const char* filename,float* result){
    ifstream in(filename);
    
    string temp;
    for(int i=0;i<TRAINED_SIZE;i++)
        in>>result[i];

    in.close();
    
    cout<<filename<<": Read done!"<<endl;
}

void WriteTo(const char* filename,float vec[]){
    ofstream out(filename);
    
    for(int i=0;i<TEST_SIZE;i++)
        out<<setprecision(4)<<fixed<<vec[i+TRAINED_SIZE]<<endl;
    
    out.close();
    
    cout<<filename<<": Write done!"<<endl;
}

void readKeySet(char* filename)
{
    ifstream in(filename);
    string buffer,temp;
    stringstream ss;
    int count = 0;
    
    //getline(in,buffer);
    //cout<<"Format: "<<buffer<<endl;
    
    for(int i=0;i<TRAINED_SIZE+TEST_SIZE;i++)
    {
        buffer.clear();
        count = 0;
        getline(in,buffer);
        ss << buffer;
        ss >> temp;
        while (!ss.eof())    //end
        {
            ss >> temp;
            em_list[i][count++] = temp;
            keySet.insert(temp);
        }
        vec_len[i] = count;
        ss.clear();  //for the next loop
    }
    
    in.close();
    
    cout<<filename<<": Read done!"<<endl;
}
