#include<iostream>
#include<string>
#include<cmath>
#include<cstring>
#include<cstdlib>
using namespace std;

const int KEY_LENGTH = 4;

int HillBox[KEY_LENGTH][KEY_LENGTH];
int InvHillBox[KEY_LENGTH][KEY_LENGTH];

// Init Hill box
void InitHillBox();
// Get Inverse HillBox
void GetInverseHillBox();
// Encrypt
string Encrypt(string text);
// Decrypt
string Decrypt(string text);
// Print HillBox
void printBox(int mat[][KEY_LENGTH]);

int main()
{
	InitHillBox();
	GetInverseHillBox();
	
	//printBox(HillBox);
	//printBox(InvHillBox);
	
	string cipherText="Hill";
	string encryptText,decryptText;
	
	encryptText = Encrypt(cipherText);
	decryptText = Decrypt(encryptText);
	
	cout<<"Cipher : "<<cipherText<<endl;
	cout<<"Encrypt: "<<encryptText<<endl;
	cout<<"Decrypt: "<<decryptText<<endl;
	
	//cout<<Encrypt("Hill")<<endl;
	//cout<<Decrypt(Encrypt("Hill"))<<endl;
	return 0;
}

// Encrypt
string Encrypt(string text)
{
	int len = text.length(),a=0,b[KEY_LENGTH];
	for(int i=0;i<len;i+=KEY_LENGTH){
		for(int j=0;j<KEY_LENGTH;j++)
		    b[j]=toupper(text[i+j])-'A';
		    
	    for(int j=0;j<KEY_LENGTH;j++)
		{
			a=0;
			for(int k=0;k<KEY_LENGTH;k++)
			{
				a += (HillBox[j][k]*b[k])%26;
			}
			
			text[i+j] = char(a%26+'A');
		}
	}
	
	return text;
}
// Decrypt
string Decrypt(string text)
{
	int len = text.length(),a=0,b[KEY_LENGTH];
	for(int i=0;i<len;i+=KEY_LENGTH){
		for(int j=0;j<KEY_LENGTH;j++)
		    b[j]=toupper(text[i+j])-'A';

	    for(int j=0;j<KEY_LENGTH;j++)
		{
			a=0;
			for(int k=0;k<KEY_LENGTH;k++)
			{
				a += (InvHillBox[j][k]*b[k])%26;
			}

			text[i+j] = char(a%26+'A');
		}
	}

	return text;
}

void InitHillBox(){
	int temp[][4]={{8,6,9,5},{6,9,5,10},{5,8,4,9},{10,6,11,4}};
	
	for(int i=0;i<KEY_LENGTH;i++)
	for(int j=0;j<KEY_LENGTH;j++)
	    HillBox[i][j]= temp[i][j];
}

void GetInverseHillBox()
{
	int temp[][4]={{23,20,5,1},{2,11,18,1},{2,20,6,25},{25,2,22,25}};

	for(int i=0;i<KEY_LENGTH;i++)
	for(int j=0;j<KEY_LENGTH;j++)
	    InvHillBox[i][j]= temp[i][j];
}

void printBox(int mat[][KEY_LENGTH])
{
	cout<<"=========="<<endl;
	for(int i=0;i<KEY_LENGTH;i++)
	{
		for(int j=0;j<KEY_LENGTH;j++)
		{
			cout<<mat[i][j]<<" ";
		}
		cout<<endl;
	}
}
