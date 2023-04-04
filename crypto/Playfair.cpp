#include<iostream>
#include<cmath>
#include<set>
#include<string>
#include<cstring>
#include<cstdlib>
using namespace std;

char PlayBox[6][6];

// Init Play box
void InitPlayBox(string key);
// Encrypt
string Encrypt(string text);
// Decrypt
string Decrypt(string text);
// Adapt the text
string Adapt(string text);

int main()
{
	InitPlayBox("FIVESTARS");
	string cipherText="Playfair cipher was actually invented by wheatstone";
	string encryptText,decryptText,adaptedText;
	
	encryptText = Encrypt(cipherText);
	decryptText = Decrypt(encryptText);
	adaptedText = Adapt(decryptText);
	
	cout<<"Cipher : "<<cipherText<<endl;
	cout<<"Encrypt: "<<encryptText<<endl;
	cout<<"Decrypt: "<<decryptText<<endl;
	cout<<"Adapted: "<<adaptedText<<endl;
	
	//cout<<"Encrypt: "<<Encrypt(cipherText)<<endl;
	//cout<<"Decrypt: "<<Decrypt(Encrypt(cipherText))<<endl;
	//cout<<"Adapted: "<<Adapt(Decrypt(Encrypt(cipherText)))<<endl;
	
	return 0;
}

// Adapt the text
string Adapt(string text){
	int len=text.length(),i;
	
	for(i=0;i<len;i++)
	{
		if(text[i]==NULL) break;
		
		if(text[i]==text[i+2]&&text[i+1]=='Q') text.erase(i+1,1);
	}
	if(text[i]=='Q') text.erase(i);
	
	return text;
}

// Encrypt
string Encrypt(string text)
{
	int len = text.length(),pi1,pj1,pi2,pj2,index=0;
	string temp;
	temp.resize(len);
	char m1,m2;
   
	len = text.length();
	for(int i=0;i<len;i+=2)
	{
	    m1=toupper(text[i]);
	    
		if(m1==' '){
			i--;
			continue;
		}else if(text[i+1]==' '){
			i++;
		}
		
		m2=i+1>len?'Q':toupper(text[i+1]);
		if(m1-m2==0) m2='Q',i--;
		
		pi1=pj1=pi2=pj2=0;
		for(int j=0;j<5;j++)
		for(int k=0;k<5;k++)
		{
			if(PlayBox[j][k]==m1) pi1=j,pj1=k;
			if(PlayBox[j][k]==m2) pi2=j,pj2=k;
			
			if(pi1+pj1!=0&&pi2+pj2!=0) break;
		}
		//cout<<pi1<<" "<<pj1<<";"<<pi2<<" "<<pj2<<endl;
		if(pi1==pi2){
			temp[index++]=PlayBox[pi1][(pj1+1)%5];
			temp[index++]=PlayBox[pi2][(pj2+1)%5];
		}else if(pj1==pj2){
			temp[index++]=PlayBox[(pi1+1)%5][pj1];
			temp[index++]=PlayBox[(pi2+1)%5][pj2];
		}else{
			temp[index++]=PlayBox[pi1][pj2];
			temp[index++]=PlayBox[pi2][pj1];
		}
	}
	
	return temp;
}

// Decrypt
string Decrypt(string text)
{
	int len = text.length(),pi1,pj1,pi2,pj2,index=0;
	string temp;
	temp.resize(len);
	char m1,m2;

	len = text.length();
	for(int i=0;i<len;i+=2)
	{
	    m1=toupper(text[i]);
		m2=toupper(text[i+1]);
		
		if(m1==NULL||m2==NULL) break;

		pi1=pj1=pi2=pj2=0;
		for(int j=0;j<5;j++)
		for(int k=0;k<5;k++)
		{
			if(PlayBox[j][k]==m1) pi1=j,pj1=k;
			if(PlayBox[j][k]==m2) pi2=j,pj2=k;

			if(pi1+pj1!=0&&pi2+pj2!=0) break;
		}
		//cout<<pi1<<" "<<pj1<<";"<<pi2<<" "<<pj2<<endl;
		if(pi1==pi2){
			temp[index++]=PlayBox[pi1][(pj1+4)%5];
			temp[index++]=PlayBox[pi2][(pj2+4)%5];
		}else if(pj1==pj2){
			temp[index++]=PlayBox[(pi1+4)%5][pj1];
			temp[index++]=PlayBox[(pi2+4)%5][pj2];
		}else{
			temp[index++]=PlayBox[pi1][pj2];
			temp[index++]=PlayBox[pi2][pj1];
		}
	}

	return temp;
}

void InitPlayBox(string key)
{
	int len = key.length(),index=0;
	string temp;
	temp.resize(26);
	set<char> used;
	for(int i=0;i<len;i++){
		if(used.count(key[i])>0) continue;
		
		temp[index++] = key[i];
		used.insert(key[i]);
	}
	
	for(char i='A';i<='Z';i++){
		if(used.count(i)>0||i=='J') continue;

		temp[index++] = i;
		used.insert(i);
	}
	
	index=0;
	for(int i=0;i<5;i++)
	for(int j=0;j<5;j++)
	{
		PlayBox[i][j] = temp[index++];
	}
}
