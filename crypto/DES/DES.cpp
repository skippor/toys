#include<cstdio>
#include<cstring>
#include<cstdlib>

#include "DES.h"

#include<iostream>
#include<string>
using namespace std;

void Fisrt_Reaplace(BIT M[64],BIT L[32],BIT R[32])
{
	for(int i = 0;i < 32;i++)
        L[i] = M[IP[i]-1];
	for(int i = 0;i < 32;i++)
        R[i] = M[IP[i+32]-1];
}

void Final_Reaplace(BIT L[32],BIT R[32],BIT M[64])
{
	int p=0;
	for(int i = 0;i < 64;i++)
	{
		p = IP_INV[i]-1;
		M[i] = p<32?L[p]:R[p-32];
	}
}

// Get Keys for DES Iterations
void Generate_Key(BIT IK[64],BIT Key[16][48])
{
	BIT CL[28],CR[28];
	PC_1_Replace(IK,CL,CR);
	
	for(int i=0;i<16;i++)
	{
		Left_Switch(CL,KM[i]);
		Left_Switch(CR,KM[i]);
		PC_2_Replace(CL,CR,Key[i]);
	}
	
}
void PC_1_Replace(BIT in[64],BIT L[28],BIT R[28])
{
	for(int i = 0;i < 28;i++)
        L[i] = in[PC_1[i]-1];
	for(int i = 0;i < 28;i++)
        R[i] = in[PC_1[i+28]-1];
}
void Left_Switch(BIT C[28],int n)
{
	BIT tmp[28];
	for(int i = 0;i < 28;i++)
        tmp[i] = (i+n<28)?C[i+n]:C[i+n-28];
	memcpy(C,tmp,sizeof(tmp));
}
void PC_2_Replace(BIT L[28],BIT R[28],BIT out[48])
{
	int p=0;
	for(int i = 0;i < 48;i++)
	{
		p = PC_2[i]-1;
		out[i] = p<28?L[p]:R[p-28];
	}
}
//DES Iterations for 16 times
void DES_Iterate(BIT L[32],BIT R[32],BIT KEY[16][48],int Mode)
{
	BIT LTEMP[32],ES[48];
	int ki=0;
	
	for(int i=0;i<16;i++)
	{
		for(int k=0;k<32;k++) LTEMP[k]=R[k];
		
		if (Mode==MODE_ENCRYPT) ki = i;
		if (Mode==MODE_DECRYPT) ki = 15-i;
		
		E_Box_Replace(R,ES);
		XOR_N(ES,KEY[ki],48);
		S_Box_Replace(ES,R);
		P_Box_Replace(R);
		XOR_N(R,L,32);

		for(int k=0;k<32;k++) L[k]=LTEMP[k];
	}
}
void XOR_N(BIT A[],BIT B[],int N)
{
	for(int i=0;i<N;i++) A[i] ^= B[i];
}
void E_Box_Replace(BIT in[32],BIT out[48])
{
	for(int i=0;i<48;i++)
	{
		out[i]=in[E[i]-1];
	}
}
void S_Box_Replace(BIT in[48],BIT out[32])
{
	int a,b,temp,is,os;
	for(int i=0;i<8;i++)
	{
		is = 6*i;
		a=(in[is]<<1)+in[is+5];
		b=(in[is+1]<<3)+(in[is+2]<<2)+(in[is+3]<<1)+(in[is+4]);
		
		temp = S[i][a][b];
		os = 4*i;
		for(int j=0;j<4;j++)
		{
			out[os+(3-j)] = (temp>>j)&1;
		}
	}
}
void P_Box_Replace(BIT X[32])
{
	BIT tmp[32];
	for(int i = 0;i < 32;i++)
	    tmp[i] = X[P[i]-1];
	memcpy(X,tmp,sizeof(tmp));
}

//--------------------------- DES -----------------------------//
// DES Process for Different Situations
//------------------- Base Struct ------------------------//
// M(64bit) + k(64bit)  =>  C(64bit)
void DES(BIT key[64],BIT M[64],BIT C[64],int Mode)
{
	BIT L[32],R[32],K[16][48];
	Generate_Key(key,K);
	Fisrt_Reaplace(M,L,R);
	DES_Iterate(L,R,K,Mode);
	Final_Reaplace(R,L,C);
}
//------------------- Base Struct ------------------------//

//--------------------------------------------------------//
//========================================================//
void TextToArray(char text[10],BIT Array[64])
{
    for(int i = 0;i < 8;i++)
        for(int j = 0;j < 8;j++)
            Array[i * 8 + (7 - j)] = (text[i] >> j)&1;
}
void ArrayToText(BIT Array[64],char text[10])
{
    for(int i = 0;i < 8;i++)
        for(int j = 0;j < 8;j++)
            text[i] |= (Array[i*8+j] << (7-j));
}
// For Test
void printArray(BIT Array[],int len)
{
    for(int i = 0;i < len;i++) cout<<Array[i];
    cout<<endl;
}
//------------ To Be Finished --------------------//
// M(64bit) + K(Matrix[16][48])  =>  C(64bit)
void DES(BIT KEY[16][48],BIT M[64],BIT C[64],int Mode)
{
	BIT L[32],R[32];
	Fisrt_Reaplace(M,L,R);
	DES_Iterate(L,R,KEY,Mode);
	Final_Reaplace(R,L,C);
}

void DES(char key[10],char text[100],char cipher[100],int Mode)
{
	BIT L[32],R[32],M[64],C[64],IK[64],K[16][48];
	char txt[10],tempText[10];
	
	TextToArray(key,IK);
	Generate_Key(IK,K);
	
	for(int i=0;text[i]!='\0';i+=8)
	{
		memset(tempText,0,sizeof(tempText));
		
		for(int j=0;j<8;j++) txt[j] = text[i+j];
		TextToArray(txt,M);
		DES(K,M,C,Mode);
		ArrayToText(C,tempText);
		
		for(int j=0;j<8;j++) cipher[i+j] = tempText[j];
	}
}

void DES_ONCE(char key[10],char text[10],char cipher[10],int Mode)
{
	BIT L[32],R[32],M[64],C[64],IK[64],K[16][48];

	TextToArray(key,IK);
	Generate_Key(IK,K);

	TextToArray(text,M);
	
	Fisrt_Reaplace(M,L,R);
	DES_Iterate(L,R,K,Mode);
	Final_Reaplace(R,L,C);
	
	memset(cipher,0,sizeof(cipher));
	ArrayToText(C,cipher);
}
//=========================================================//
//---------------- string realize -------------------------//
string DES(string key,string text,int Mode)
{
	int len = text.length();
	string cipher;
	cipher.resize(len+8);

	BIT L[32],R[32],M[64],C[64],IK[64],K[16][48];
	char txt[10],tempText[10];
	
	for(int i = 0;i < 8;i++)
        for(int j = 0;j < 8;j++)
            IK[i * 8 + (7 - j)] = (key[i] >> j)&1;

	Generate_Key(IK,K);
	
	for(int i=0;i<len;i+=8)
	{
		memset(tempText,0,sizeof(tempText));

		for(int j=0;j<8;j++) txt[j] = text[i+j];
		TextToArray(txt,M);
		DES(K,M,C,Mode);
		ArrayToText(C,tempText);

		for(int j=0;j<8;j++) cipher[i+j] = tempText[j];
	}

	return cipher;
}

void Test()
{
	string k = "1234567",t="aa b cc d ee f gg ?";
	cout<<"DES:"<<DES(k,t,MODE_ENCRYPT)<<endl
	    <<DES(k,DES(k,t,MODE_ENCRYPT),MODE_DECRYPT)<<endl;
	
	char key[10];
	char text[100];
	char cipher[100]={0};

	cout<<"Input your key(8byte):";
	cin>>key;
	cout<<"Input your text:";
	cin>>text;
	DES(key,text,cipher,MODE_ENCRYPT);
	cout<<"Encrypt Result is:"<<cipher<<endl;
	DES(key,cipher,text,MODE_DECRYPT);
	cout<<"Decrypt Result is:"<<text<<endl;
	
//	cout<<"E for Encrypt|D for Decrypt:";
//	cin>>O;

//	if(O[0]=='E'){
//		DES(key,text,cipher,MODE_ENCRYPT);
//		cout<<"Result is:"<<cipher<<endl;
//	}
//	if(O[0]=='D'){
//		DES(key,text,cipher,MODE_DECRYPT);
//		cout<<"Result is:"<<cipher<<endl;
//	}
	cout<<endl;
}
int main()
{
	Test();
	
	char a[10]={"program"};
	char b[10]={"a b c ?"};
	char b2[100]={"How to use computer?"};
	char temp[100],d[100]={0};
	//memset(temp,0,sizeof(temp));
	//memset(d,0,sizeof(d));
	
    cout<<"Hello DES!"<<endl;
	DES(a,b2,temp,MODE_ENCRYPT);
	cout<<"DES E: "<<temp<<endl;
	DES(a,temp,d,MODE_DECRYPT);
	cout<<"DES D: "<<d<<endl;
	
    //cout<<"Hello DES!"<<endl;
	//DES_ONCE(a,b,temp,MODE_ENCRYPT);
	//cout<<"DES E: "<<temp<<endl;
	//DES_ONCE(a,temp,d,MODE_DECRYPT);
	//cout<<"DES D: "<<d<<endl;

	return 0;
}



