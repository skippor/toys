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

const int DIMENSION = 2;
const int TEST_SIZE = 788;
const int MIN_PTS = 100;
const float RADIUS = 8;
//global definations

class Point
{
	public:
		Point();
		~Point();
		void Print();
		void getNeighbors(Point pSet[]);
		float getEulerDistance(Point& p);
	
		int id;
		int classID;
		bool visited;
		bool isCore;
		vector<int> neighborID;//TEST_SIZE
		float point[DIMENSION];
};
Point::Point(){
	classID=0;
}
Point::~Point(){
	neighborID.clear();
}
void Point::Print(){
    cout<<"id:"<<id<<",Core:"<<isCore<<",classID:"<<classID<<"-->";
	cout<<"("<<point[0];
	for(int j=1;j<DIMENSION;j++)
	   cout<<", "<<point[j];
	cout<<")"<<endl;
}
float Point::getEulerDistance(Point& p){
	float sum = 0;

    for(int i=0;i<DIMENSION;i++)
    {
        sum+=(point[i]-p.point[i])*(point[i]-p.point[i]);
    }
    return sqrt(sum);
}
void Point::getNeighbors(Point p[]){
	float dis=0;
	for (int i=0;i<TEST_SIZE;i++)
	{
		dis = this->getEulerDistance(p[i]);
		if(dis<=RADIUS && this->id!=p[i].id)
		{
			neighborID.push_back(i);
		}
	}
	
	if(neighborID.size()>=MIN_PTS)
	{
		isCore = true;
	}
}

//float point[TEST_SIZE+5][DIMENSION];        // 点集数据

void ReadData(const char* filename,Point* p);
void WriteTo(const char* filename,vector<int> p[],int length);
void Expand(Point p[],int pid,int cid);
void DBSCAN_solution(Point p[]);

int main()
{
	//int noise[TEST_SIZE],core[TEST_SIZE];
	Point p[TEST_SIZE];

    //ReadData("DBSCAN_DataSet.txt",p);
	//测试集：http://blog.sina.com.cn/s/blog_4ada5be50100uj7a.html
	
    ReadData("788points_DataSet.txt",p);
    
    for (int i=0;i<TEST_SIZE;i++)
	{
		p[i].getNeighbors(p);
		//p[i].Print();
	}
	
	DBSCAN_solution(p);
	//for (int i=0;i<TEST_SIZE;i++) p[i].Print();
	
    //WriteTo("dbscan_result.txt");
	return 0;
}

void ReadData(const char* filename,Point* p){
    ifstream in(filename);

    for(int i=0;i<TEST_SIZE;i++)
    {
    	p[i].id=i;
    	p[i].isCore=false;
    	p[i].visited=false;
    	for(int j=0;j<DIMENSION;j++)
        {
            in>>p[i].point[j];
        }
    }

    in.close();
    cout<<filename<<": Read done!"<<endl;
}

void WriteTo(const char* filename,vector<int> p[],int length){
    ofstream out(filename);
    vector<int> temp;
    for(int i=0;i<length;i++)
    {
    	temp=p[i];
    	if(temp.empty()) continue;
    	
		out<<"Class "<<i<<": "<<endl;
		
		out<<"("<<temp[0];
		for(int j=1;j<temp.size();j++)
		   out<<", "<<temp[j];
		out<<")"<<endl;
		
		out<<endl;
    }

    out.close();
    cout<<filename<<": Write done!"<<endl;
}

void DBSCAN_solution(Point p[]){
	cout<<"Get DBSCAN solution:";
	int class_ID=1;
	vector<int> p_index[TEST_SIZE];
	
	for(int i=0;i<TEST_SIZE;i++)
	{
		if(!p[i].visited && p[i].isCore)
		{
			p[i].classID=class_ID;
			p[i].visited=true;
			Expand(p,p[i].id,class_ID);
			class_ID++;
		}
	}
	
	for(int i=0;i<TEST_SIZE;i++)
	{
		//if(p[i].visited)
        p_index[p[i].classID].push_back(p[i].id);
	}
	cout<<" Done!"<<endl;
	
	WriteTo("dbscan_result.txt",p_index,class_ID);
}

void Expand(Point p[],int pid,int cid){
	Point pnow = p[pid];
	//pnow.visited=true;
	if(!pnow.isCore) return;
	
	int len=pnow.neighborID.size(),nid=0;
	for(int i=0;i<len;i++)
	{
		nid = pnow.neighborID[i];
		if(!p[nid].visited)
		{
			p[nid].classID=cid;
			p[nid].visited = true;
			
			if(p[nid].isCore)
			{
				Expand(p,p[i].id,cid);
			}
		}
	}
}


