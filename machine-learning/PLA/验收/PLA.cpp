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

const int TRAINED_SIZE = 6;
const int TEST_SIZE = 4;
const int COLUMNS = 2;

//global definations
float train_data[TRAINED_SIZE][COLUMNS+5];
float test_data[TEST_SIZE][COLUMNS+5];
int shares[TRAINED_SIZE+TEST_SIZE+5];
float w[COLUMNS+2]={0};
float wtemp[COLUMNS+2]={0};

//Read data of the trained files
void ReadTrainedData();
//Read data of the test files
void ReadTestData();
//Write the rate data to test files
void WriteTo(const char* filename);
//sign return (value>0)?1:-1;
int Sign(float value);
//Get the weight matrix w
void GetMatrixW();
//Predict the shares of test data
void Predict();
//Get PLA Solution
void PLA_Solution();

//Get the mistake checked numbers of trained data
//--------------- Pocket PLA ---------------------//
int GetMistakeNumber(float weight[]);
//Get the weight matrix w by pocket PLA
void GetPocketW();
//--------------- Pocket PLA ---------------------//



int main()
{
	ReadTrainedData();
    ReadTestData();
	    
	PLA_Solution();

    WriteTo("prediction.txt");
    
	return 0;
}

void PLA_Solution()
{
//  GetPocketW();
	GetMatrixW();
	for(int j=0;j<=COLUMNS;j++) cout<<w[j]<<" ";cout<<endl;
	cout<<"mistake: "<<GetMistakeNumber(w)<<endl;
	
	Predict();
    //cout<<Sign(0)<<endl;
}

void Predict()
{
    cout<<"Predict begin!"<<endl;
	float h=0;
	for(int i=0;i<TEST_SIZE;i++)
    {
        cout<<"for test "<<i+1<<":"<<endl;
    	h=0;
        for(int j=0;j<=COLUMNS;j++)  h += w[j]*test_data[i][j];
        
        shares[TRAINED_SIZE+i]=Sign(h);
        cout<<h<<" "<<Sign(h)<<" "<<shares[TRAINED_SIZE+i]<<endl;
    }
    
    cout<<"Predict Done!"<<endl;
}

int GetMistakeNumber(float weight[])
{
	float h=0;
	int count=0;
	for(int i=0;i<TRAINED_SIZE;i++)
	{
		h=0;
		for(int j=0;j<=COLUMNS;j++)
		{
			h += weight[j]*train_data[i][j];
		}
		if(Sign(h) != shares[i])
			count++;
	}
	
	return count;
}

void GetPocketW()
{
	cout<<"Get Pocket Matrix W:";
	memset(w,1,sizeof(w));
	memset(wtemp,1,sizeof(wtemp));

	float h=0;
 	int iterations=100000,mistake,newmistake,index;
	srand(time(0));
	mistake = GetMistakeNumber(w);
    
	while(iterations--){
		h=0;
		index = (int)((TRAINED_SIZE-1)*rand()/RAND_MAX);
		
		for(int j=0;j<=COLUMNS;j++)
		{
			h += w[j]*train_data[index][j];
		}
		//cout<<Sign(h)<<shares[i]<<endl;
		if(Sign(h) != shares[index])
		{
			for(int j=0;j<=COLUMNS;j++) wtemp[j] = w[j]+shares[index]*train_data[index][j];
			
			newmistake = GetMistakeNumber(wtemp);
			//cout<<newmistake<<endl;
			if(newmistake<=mistake)
			{
				mistake = newmistake;
				for(int j=0;j<=COLUMNS;j++) w[j] = wtemp[j];
			}
		}
    }
    
	cout<<" Done!"<<endl;
}

void GetMatrixW()
{
	cout<<"Get Matrix W:";
	memset(w,1,sizeof(w));
	
	float h=0;
	int max_count=10000,count=0;
	for(int i=0;i<TRAINED_SIZE;i++)
	{
		h=0;
		for(int j=0;j<=COLUMNS;j++)
		{
			h += w[j]*train_data[i][j];
		}
		//cout<<Sign(h)<<shares[i]<<endl;
		if(Sign(h) != shares[i])
		{
			for(int j=0;j<=COLUMNS;j++) w[j] = w[j]+shares[i]*train_data[i][j];
			
			i=0;
			count++;
			if(count==max_count) break;
		}
	}
	cout<<" Done!"<<endl;
}

//sign return (value>0)?1:-1;
int Sign(float value){
	if (value>0) return 1;
	//else if(value<0) return 0;
	else return -1;
}

//Write the rate data to test files
void WriteTo(const char* filename){
    ofstream out(filename);

    for(int i=TRAINED_SIZE;i<TRAINED_SIZE+TEST_SIZE;i++)
        out<<shares[i]<<endl;

    out.close();

    cout<<filename<<": Write done!"<<endl;
}

//Read data of the trained files
void ReadTrainedData(){
    cout<<"Read Trained Data:";
    
    ifstream in("train.txt");
    string buffer;
    float temp;
    //getline(in,buffer);
    for(int i=0;i<TRAINED_SIZE;i++)
    {
        for(int j=0;j<COLUMNS;j++)
        {
            in>>temp;
            train_data[i][j] = temp;
            
        }
        train_data[i][COLUMNS]=1;
        in>>shares[i];
    }
        
    in.close();

    cout<<" Done!"<<endl;
}

//Read data of the test files
void ReadTestData(){
    cout<<"Read Test Data:";
    ifstream in("test.txt");
    string buffer;
    float temp;
    //getline(in,buffer);
    for(int i=0;i<TEST_SIZE;i++)
    {
        for(int j=0;j<COLUMNS;j++)
        {
            in>>temp;
            test_data[i][j] = temp;
            
        }
        test_data[i][COLUMNS]=1;
    }

    in.close();

    cout<<" Done!"<<endl;
}

