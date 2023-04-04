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

const int TRAINED_SIZE = 27751;
const int TEST_SIZE = 11893;
const int COLUMNS = 58;
const float LP_BASE = 1e-3;

//global definations
float train_data[TRAINED_SIZE][COLUMNS+5];
float test_data[TEST_SIZE][COLUMNS+5];
int shares[TRAINED_SIZE+TEST_SIZE+5];

float CMAX[COLUMNS+5] = {0},Sum[COLUMNS+5] = {0};   //For normalization to reduce influence caused by value

//Read data of the trained files
void ReadTrainedData();
//Read data of the test files
void ReadTestData();
//Write the rate data to test files
void WriteTo(const char* filename);
//To Normalize the data Set
void Normalize();
//Get the Euler diatance
float getEulerDistance(float vec1[],float vec2[],int length);
//Get the City block distance
float getCityblockdistance(float vec1[],float vec2[],int length);


//Get KNN Rate --- k=1
void GetKNNRate();
//Get IDW Rate --- stronger version of KNN
void GetIDWRate();
//Get NB rate
void GetNBRate();
//Get Solution
void GetSolution();


int main()
{
    GetSolution();
    
	return 0;
}

void GetSolution()
{
    ReadTrainedData();
    
    ReadTestData();
    
    Normalize();
    
    GetKNNRate();
    //GetNBRate();
    
    WriteTo("prediction.txt");
}



//Get NB rate
void GetNBRate(){
    cout<<"Get NB Rate:";

    float prediction = 0,sum,sum1;

    for(int i=0;i<TEST_SIZE;i++)
    {
        prediction = 0;
        for(int j=0;j<TRAINED_SIZE;j++)
        {
            sum = 0,sum1=0;
            for (int k=0;k<COLUMNS;k++)
            {
                sum += test_data[i][k];
                sum1+= train_data[j][k];
            }
            
            prediction += shares[j]*sum*sum1;
        }

        shares[TRAINED_SIZE+i] = prediction;
    }

    cout<<" Done!"<<endl;
}

//Get KNN Rate --- k=1
void GetKNNRate(){
    cout<<"Get KNN Rate:";
    
    int index = 0;
    float min = 100,distance = 0;
    
    for(int i=0;i<TEST_SIZE;i++)
    {
        min = 100;
        index = 0;
        for(int j=0;j<TRAINED_SIZE;j++)
        {
            // TODO:getCityblockdistance
            distance = getEulerDistance(test_data[i],train_data[j],COLUMNS);
            
            if(distance<min)
            {
                min = distance;
                index = j;
            }
        }
        
        
        shares[TRAINED_SIZE+i] = shares[index];
    }
    
    cout<<" Done!"<<endl;
}

//To Normalize the data
void Normalize(){
    cout<<"Normalization:";
    
    for(int j=0;j<COLUMNS;j++)
    {
        for(int i=0;i<TEST_SIZE;i++)
        {
            test_data[i][j] /= Sum[j];//CMAX[j];
            train_data[i][j] /= Sum[j];//CMAX[j];
        }
        for(int i=TEST_SIZE;i<TRAINED_SIZE;i++)
        {
            train_data[i][j] /= Sum[j];//CMAX[j];
        }
    }
    
    cout<<" Done!"<<endl;
}

//Get the Euler diatance
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
    
    ifstream in("Datar_train.txt");
    string buffer;
    float temp;
    getline(in,buffer);
    for(int i=0;i<TRAINED_SIZE;i++)
    {
        for(int j=0;j<COLUMNS;j++)
        {
            in>>temp;
            train_data[i][j] = temp;
            
            Sum[j] += temp;
            if(temp>CMAX[j])
                CMAX[j] = temp;
        }
           
        in>>shares[i];
    }
        
    in.close();

    cout<<" Done!"<<endl;
}

//Read data of the test files
void ReadTestData(){
    cout<<"Read Test Data:";
    ifstream in("Datar_test.txt");
    string buffer;
    float temp;
    getline(in,buffer);
    for(int i=0;i<TEST_SIZE;i++)
    {
        for(int j=0;j<COLUMNS;j++)
        {
            in>>temp;
            test_data[i][j] = temp;
            
            Sum[j] += temp;
            if(temp>CMAX[j])
                CMAX[j] = temp;
        }
    }

    in.close();

    cout<<" Done!"<<endl;
}

