#include<iostream>
#include<cmath>
using namespace std;

char VigBox[30][30];

// Init Vig box
void InitVigBox();
// Beaufort Method
void Beaufort();
// Encrypt
string Encrypt(string key,string text);
// Decrypt
string Decrypt(string key,string cipher);


int main()
{
	string key = "best",cipherText="datasecurity";
	InitVigBox();
	
	cout<<"Cipher Text: "<<cipherText<<endl;
	cout<<"Encrypt: "<<Encrypt(key,cipherText)<<endl;
	cout<<"Decrypt: "<<Decrypt(key,Encrypt(key,cipherText))<<endl;
	
	return 0;
}


// Encrypt
string Encrypt(string key,string text)
{
	int tlen=text.length(),klen=key.length(),ki=0,x=0,y=0;
	
	for(int i=0;i<tlen;i++)
	{
		x=(int)(key[ki]-'a');
		y=(int)(text[i]-'a');
		text[i]=VigBox[x][y];
		
		ki=(++ki>=klen)?0:ki;
	}
	
	return text;
}
// Decrypt
string Decrypt(string key,string text)
{
	int tlen=text.length(),klen=key.length(),ki=0,x=0,y=0;

	for(int i=0;i<tlen;i++)
	{
		x=(int)(key[ki]-'a');
		y=(text[i]-VigBox[x][0]+26)%26;
		text[i]='a'+y;

		ki=(++ki>=klen)?0:ki;
	}

	return text;
}

// Init Vig box
void InitVigBox(){
	int Start = (int)'A',temp;
	for(int i=0;i<26;i++)
	{
		temp=Start;
		for(int j=0;j<26;j++)
		{
			VigBox[i][j] = char(temp);
			temp=(++temp>'Z')?'A':temp;
		}
		Start ++;
	}
}

void Beaufort(){
	int Start = (int)'Z',temp;
	for(int i=0;i<26;i++)
	{
		temp=Start;
		for(int j=0;j<26;j++)
		{
			VigBox[i][j] = char(temp);
			temp = 'A'+(26+temp-'A'-1)%26;
		}
		Start = 'A'+(Start+1-'A')%26;
	}
}
