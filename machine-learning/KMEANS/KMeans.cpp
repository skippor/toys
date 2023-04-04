#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<string>
#include<set>
#include<vector>
#include<algorithm>
#include<cmath>
#include<ctime>
#include<cstring>
#include<cstdlib>

using namespace std;

const int DIMENSION = 2;
const int TEST_SIZE = 788;
const int CLASS_NUMBER = 7;
const float EPS=1e-8;

//global definations
bool stop=false;
float point[TEST_SIZE+5][DIMENSION];        // 点集数据
int class_index[CLASS_NUMBER][TEST_SIZE]; // 类标签名
int class_length[CLASS_NUMBER]={0};           // 类长度
float center_point[CLASS_NUMBER][DIMENSION];//={{1,1,1},{10,10,1}};   // 初始中心点

float getEulerDistance(float point1[],float point2[]);
void ReadData(const char* filename);
void WriteTo(const char* filename);
void KMeans_Solution();
int Mindex(float distance[]);
void ReCalCenterPoint();
void PrintPoint(float point[][DIMENSION],int length);
void PrintResult();
void PrintCenterPoint();
void IniCenterPoint();

int main()
{
    //cout<<"OK"<<endl;
    //ReadData("KMeans_DataSet.txt");
    ReadData("788points_DataSet.txt");
    
    //PrintPoint(point,TEST_SIZE);
    //KMeans_Solution();
    IniCenterPoint();
    int number=0;
    //for(int i=0;i<10;i++)
    while(!stop)
    {
        cout<<endl;
        PrintCenterPoint();
        KMeans_Solution();
        //PrintResult();
        ReCalCenterPoint();

		//if(stop) break;
		number++;
    }  
    
    cout<<"Loop : "<<number<<" times!"<<endl;
    WriteTo("kmeans_result.txt");
	return 0;
}

void IniCenterPoint()
{
	srand(time(0));
	int count=0,index=0,index_set[TEST_SIZE]={0};
	while(count<CLASS_NUMBER)
	{
		index = (int)rand()%TEST_SIZE;
		if (index_set[index]==1)
			continue;
			
		center_point[count++][DIMENSION]=point[index][DIMENSION];
		index_set[index]=1;
	}
}

void KMeans_Solution()
{
    for(int j=0;j<CLASS_NUMBER;j++) class_length[j]=0;
    
    float dis[CLASS_NUMBER] = {0};
    int index = 0;
    
    for(int i=0;i<TEST_SIZE;i++)
    {
        for(int j=0;j<CLASS_NUMBER;j++)
        {
            dis[j]=getEulerDistance(point[i],center_point[j]);
        }
        index = Mindex(dis);
        class_index[index][class_length[index]] = i;
        class_length[index]++;
    }
}

float getEulerDistance(float point1[],float point2[]){
    float sum = 0;
    
    for(int i=0;i<DIMENSION;i++)
    {
        sum+=(point1[i]-point2[i])*(point1[i]-point2[i]);
    }
    return sqrt(sum);
}

void ReadData(const char* filename){
    ifstream in(filename);

    for(int i=0;i<TEST_SIZE;i++)
        for(int j=0;j<DIMENSION;j++)
        {
            in>>point[i][j];
        }

    in.close();
    
    cout<<filename<<": Read done!"<<endl;
}

void WriteTo(const char* filename){
    ofstream out(filename);
    int c_len = 0,cur=0;
    out<<"There are "<<CLASS_NUMBER<<" Classes which are:"<<endl;
    for(int i=0;i<CLASS_NUMBER;i++)
    {
        out<<endl<<"Class "<<i+1<<" :"<<endl;
        c_len = class_length[i];
        for (int j=0;j<c_len;j++)
        {
            cur = class_index[i][j];
            out<<"("<<point[cur][0];
            for(int k=1;k<DIMENSION;k++)
            {
                out<<", "<<point[cur][k];
            }
            out<<")"<<endl;
        }
    }
    out.close();
    
    cout<<filename<<": Write done!"<<endl;
}

void PrintPoint(float point[][DIMENSION],int length)
{
    for(int i=0;i<length;i++)
    {
        cout<<"("<<point[i][0];
        for(int j=1;j<DIMENSION;j++)
           cout<<", "<<point[i][j];

        cout<<")"<<endl;
    }
}

int Mindex(float distance[]){
    int min = 0xffff,index=0;
    
    for(int i=0;i<CLASS_NUMBER;i++)
    {
        if(distance[i]<min)
        {
            index = i;
            min = distance[i];
        }
    }
    return index;
}

void PrintResult()
{
    int c_len = 0,cur=0;
    for(int i=0;i<CLASS_NUMBER;i++)
    {
        cout<<"Class "<<i+1<<" :"<<endl;
        c_len = class_length[i];
        for (int j=0;j<c_len;j++)
        {
            cur = class_index[i][j];
            cout<<"("<<point[cur][0];
            for(int k=1;k<DIMENSION;k++)
            {
                cout<<", "<<point[cur][k];
            }
            cout<<")"<<endl;
        }
    }
}

void ReCalCenterPoint()
{
    int c_len = 0,cur=0;
    float avg = 0,temp;
	stop=true;
    for(int i=0;i<CLASS_NUMBER;i++)
    {
        c_len = class_length[i];
        for(int j=0;j<DIMENSION;j++)
        {
            avg = 0;
            for (int k=0;k<c_len;k++)
            {
                cur = class_index[i][k];

                avg += point[cur][j];
            }
            temp =avg/(float)c_len;
            if(abs(center_point[i][j]-temp)<EPS)
                center_point[i][j]=temp;
            else
            {
            	center_point[i][j]=temp;
            	stop=false;
            }
                
        }
    }
}

void PrintCenterPoint()
{
    cout<<"Center Point:";
    for(int i=0;i<CLASS_NUMBER;i++)
    {
        cout<<"("<<center_point[i][0];
        for(int j=1;j<DIMENSION;j++)
        {
            cout<<", "<<center_point[i][j];
        }
        cout<<") ";
    }
    cout<<endl;
}

