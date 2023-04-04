#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<cmath>
#include<iomanip>
#include<algorithm>

using namespace std;
const int DataRow=12;
const int DataColumn=17;
struct Node
{
	double value;//代表此时yes的概率。
	int attrid;
	Node * parentNode;
	vector<Node*> childNode;
};

string DataTable[DataRow][DataColumn];
map<string,int> str2int;
set<int> S;
set<int> Attributes;
string attrName[DataColumn]={"ID","1","2","4","5","7","9","12","15","16","17","20","25","26","27","39","E"};
string attrValue[DataColumn][DataRow]=
{
	{},//D1,D2这个属性不需要
	{"1","0"},{"1","0"},{"1","0"},{"1","0"},{"1","0"},
	{"1","0"},{"1","0"},{"1","0"},{"1","0"},{"1","0"},
	{"1","0"},{"1","0"},{"1","0"},{"1","0"},{"1","0"}
};
int attrCount[DataColumn]={12,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4};


//----------- for lab4.txt -----------------//
const int SIZE = 12;

//global definations
set<int> keySet;
int AUs[15][10],emotion[15];
int vec_len[15]={0};
int dataVec[15][20]={{0}};

// Read the dataset words
void readKeySet(char* filename);
//Read the data of the files
void readDataFrom(const char* filename,int*);
//To vectorize the data Set
void Vectorize(int);
//Write vectorized data to file
void WriteTo(const char* filename);

//------------------ ID3 ---------------------//

void Init();
double lg2(double n);
double Entropy(const set<int> &s);
double Gain(const set<int> & example,int attrid);
int FindBestAttribute(const set<int> & example,const set<int> & attr);
Node * Id3_solution(set<int> example,set<int> & attributes,Node * parent);

int main()
{
	readKeySet((char*)"Dataset.txt");
    readDataFrom((char*)"Emotion.txt",emotion);
    int vec_length = keySet.size();
    Vectorize(vec_length);
    WriteTo("lab4.txt");
    
	Init();
	Node * Root=Id3_solution(S,Attributes,NULL);
	return 0;
}


void Init()
{
	ifstream fin("lab4.txt");
	for(int i=0;i<DataRow;i++)
	{
	  for(int j=0;j<DataColumn;j++)
	  {
		  fin>>DataTable[i][j];
	  }
	}
	fin.close();
	for(int i=1;i<DataColumn;i++)
	{
		str2int[attrName[i]]=i;
		for(int j=0;j<attrCount[i];j++)
		{
			str2int[attrValue[i][j]]=j;
		}
	}
	for(int i=0;i<DataRow;i++)
	  S.insert(i);
	for(int i=1;i<=DataColumn-2;i++)
	  Attributes.insert(i);
}

double lg2(double n)
{
	return log(n)/log(2);
}
double Entropy(const set<int> &s)
{
	double a7=0,a6=0,a4=0,a3=0,sum=s.size(),ans=0;
	for(set<int>::iterator it=s.begin();it!=s.end();it++)
	{
		string s=DataTable[*it][str2int["E"]];
		if(s=="7")
		  a7++;
		else if(s=="6")
		  a6++;
		else if(s=="4")
		  a4++;
        else
		  a3++;
	}
	if(a7==sum||a6==sum||a4==sum||a3==sum)
	  return ans=0;
    if (a7!=0)
	  ans-=a7/sum*lg2(a7/sum);
	if (a6!=0)
	  ans-=a6/sum*lg2(a6/sum);
    if (a4!=0)
	  ans-=a4/sum*lg2(a4/sum);
	if (a3!=0)
	  ans-=a3/sum*lg2(a3/sum);

	return ans;
}
double Gain(const set<int> & example,int attrid)
{
	int attrcount=attrCount[attrid];
	double ans=Entropy(example);
	double sum=example.size();
	set<int> * pset=new set<int>[attrcount];
	for(set<int>::iterator it=example.begin();it!=example.end();it++)
	{
		pset[str2int[DataTable[*it][attrid]]].insert(*it);
	}
	for(int i=0;i<attrcount;i++)
	{
		ans-=pset[i].size()/sum*Entropy(pset[i]);
	}
	return ans;
}
int FindBestAttribute(const set<int> & example,const set<int> & attr)
{
	double mx=0;
	int k=-1;
	for(set<int>::iterator i=attr.begin();i!=attr.end();i++)
	{
		double ret=Gain(example,*i);
		if(ret>mx)
		{
			mx=ret;
			k=*i;
		}
	}

	if(k==-1)
	  cout<<"FindBestAttribute error!"<<endl;
	return k;
}
Node * Id3_solution(set<int> example,set<int> & attributes,Node * parent)
{
	Node *now=new Node;//创建树节点。
	now->parentNode=parent;
	if(attributes.empty())//如果此时属性列表已用完，即为空，则返回。
	  return now;

	/*
	 * 统计一下example，如果都为正或者都为负则表示已经抵达决策树的叶子节点
	 * 叶子节点的特征是有childNode为空。
	 */
	int a7=0,a6=0,a4=0,a3=0,sum=example.size();
	for(set<int>::iterator it=example.begin();it!=example.end();it++)
	{
		string s=DataTable[*it][str2int["E"]];
		if(s=="7")
		  a7++;
		else if(s=="6")
		  a6++;
		else if(s=="4")
		  a4++;
        else
		  a3++;
	}
	if(a7==sum||a6==sum||a4==sum||a3==sum)
	{
		if(a7==sum)
		{
			now->value = 7;
		}

		if(a6==sum)
		{
			now->value = 6;
		}

		if(a4==sum)
		{
			now->value = 4;
		}

		if(a3==sum)
		{
			now->value = 3;
		}

		return now;
	}


	/*找到最高信息增益的属性并将该属性从attributes集合中删除*/
	int bestattrid=FindBestAttribute(example,attributes);
	//cout<<"best:"<<bestattrid<<endl;
	now->attrid=bestattrid;
	attributes.erase(attributes.find(bestattrid));

	/*将exmple根据最佳属性的不同属性值分成几个分支，每个分支有即一个子树*/
	vector< set<int> > child=vector< set<int> >(attrCount[bestattrid]);
	for(set<int>::iterator i=example.begin();i!=example.end();i++)
	{
		int id=str2int[DataTable[*i][bestattrid]];
		child[id].insert(*i);
	}
	for(int i=0;i<child.size();i++)
	{
		Node * ret=Id3_solution(child[i],attributes,now);
		now->childNode.push_back(ret);
	}
	return now;
}

void Vectorize(int length){
    cout<<"Vectorization:";

    int position,len=0;
    set<int>::iterator begin = keySet.begin();
    set<int>::iterator it;

    for(int i=0;i<SIZE;i++)
    {
        len = vec_len[i];

        for(int j=0;j<len;j++)
        {
            it = keySet.find(AUs[i][j]);
            position = std::distance(begin,it);
            dataVec[i][position]=1;

            //vec_position[i][j] = position; //store position here for later calculations
        }
    }

    cout<<" Done!"<<endl;
}


void readDataFrom(const char* filename,int* result){
    ifstream in(filename);

    string header,temp;
    getline(in,header);
    for(int i=0;i<SIZE;i++)
        in>>temp>>result[i];

    in.close();

    cout<<filename<<": Read done!"<<endl;
}

void readKeySet(char* filename){
    ifstream in(filename);
    string buffer,temp;
    stringstream ss;
    int count = 0,temp_e;

    getline(in,buffer);
    //cout<<"Format: "<<buffer<<endl;

    for(int i=0;i<SIZE;i++)
    {
        buffer.clear();
        count = 0;
        getline(in,buffer);
        ss << buffer;
        ss >> temp;
        while (!ss.eof())    //end
        {
            ss >> temp_e;
            AUs[i][count++] = temp_e;
            keySet.insert(temp_e);
        }
        vec_len[i] = count;
        ss.clear();  //for the next loop
    }

    in.close();

    cout<<filename<<": Read done!"<<endl;
}

void WriteTo(const char* filename){
    ofstream out(filename);

    for(int i=0;i<SIZE;i++)
    {
        out<<"Train"<<i+1<<"\t";
        for(int j=0;j<keySet.size();j++)
        {
            out<<dataVec[i][j]<<" ";
        }
        out<<emotion[i]<<endl;
    }
    
    out.close();

    cout<<filename<<": Write done!"<<endl;
}
