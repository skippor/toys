#include <iostream>
#include <string.h>
using namespace std;
#define c1 9
#define c2 5
class Apriori
{
public:
	Apriori();
	void apriori_gen();
	int Substr(char p1[c2],char p2[c2]);//�ж�p2��Ԫ���Ƿ���p1��
	int Join(char p1[c2],char p2[c2],char p3[c2][c2]);//���Ӻ���
	int Count_support(char candidate[c1]);//����֧�ֶ�

	void print();
	void Associat(char Frequent[25][c2]);

private:
	char *Candidate[c1];
	char *p[c1];
};

Apriori::Apriori()
{
	for(int j=0;j<c1;j++)
	Candidate[j]=(char*)"00000";
	
	p[0]=(char*)"12500";
	p[1]=(char*)"24000";
	p[2]=(char*)"23000";
	p[3]=(char*)"12400";
	p[4]=(char*)"13000";
	p[5]=(char*)"23000";
	p[6]=(char*)"13000";
	p[7]=(char*)"12350";
	p[8]=(char*)"12300";
}

void Apriori::print()
{
	for(int i=0;i<c1;i++)
	{cout<<"\n";
		for(int j=0;j<c2;j++)
		cout<<*(p[i]+j);
		}
	for(int i=0;i<c1;i++)
	{cout<<"\n";
		for(int j=0;j<c2;j++)
		cout<<*(Candidate[i]+j);
		}

	cout<<endl;
}

int Apriori::Join(char p1[c2],char p2[c2],char p3[c2][c2])//t2��ʾp3�е�����
{	int t1=0;int t2=-1;
	char t3;
	int s;
	int h=0;
	for(int k=0;k<c2;k++)
		{
			for(int i=0;i<c2;i++)
				p3[k][i]='0';
		}
	for(int j=0;j<c2;j++)	
	{
		if((p1[j])=='0')//t1��ʾp1�з���Ԫ�ظ���
		{t1=j-1;
		break;}
	}
	for(int j=0;j<=t1;j++)
	{	
		while(h<=t1)
		{if((p1[h])==p2[j])
			{h=0;s=1;break;}
		else {s=0; h++;}
			}	
		if(s==0)
			{t3=p2[j];
            t2++;
			for(int k=0;k<=t1;k++)
			p3[t2][k]=p1[k];
			p3[t2][t1+1]=t3;
			h=0;
			}	
	}
	return(t2+1);
}	
int Apriori::Substr(char p1[c2],char p2[c2])
{
	int i=0,j=0,y=0,x=0;
	int len1,len2;
	len1=strlen(p1);
	len2=strlen(p2);
	while((i<len2)&&(p2[i]!='0'))
	{	if(x==1)
			return(y);
		else
			if((i>=len2))
			return(y);
		while((j<=len1)&&(p1[j]!='0'))
		{if(((p2[i])==p1[j]))
			{j=0; i++; y=1; break;}
		else
			{y=0; j++;
		if(p1[j]=='0') x=1;}
		}
	}
	return(y);
}
int Apriori::Count_support(char candidate[c2])
{
	int i=0;int supportcount=0;
	for(i=0;i<9;i++)
		if(Substr(p[i],candidate))
			supportcount++;
	return(supportcount);
}
void Apriori::Associat(char Frequent[25][c2])
{	char Candidate[c1+10][c2];//��ʼ��ѡ��
	char temp[c2][c2];
	int next[c2];//������¼kƵ�����Ԫ�صĸ���
	int flag=c2;//�жϽ������ò��ٲ���kƵ���Ϊֹ
	next[0]=5;
	int i,j,k,m,n=5;
	int r=0;
	int f=0;
	char s='0';
	for(i=1;i<c2;i++)
		next[i]=0;
	for(i=0;i<c2;i++)
		for(j=0;j<c2;j++)
			{if(j==0)
				Candidate[i][j]=++s;
			else
				Candidate[i][j]='0';}
	for(i=0,r=0;(i<c2)&&(flag!=0);i++,r++)//����k��Ĳ���
	{
		for(j=0;j<next[r];j++)//��k-1Ƶ�������kƵ���
		{/////////////////////////////////////////////////////////////////////////////////////////
			for(k=j+1;k<next[r];k++)
			{
				n=Join(Candidate[j],Candidate[k],temp);
				next[r+1]=next[r+1]+n;
				for(m=0;m<n;m++)//������kƵ�������Frenquent��
				{
					if(Count_support(temp[m])>=2)
					{
						for(int fz=0;fz<c2;fz++)
						Frequent[f][fz]=temp[m][fz];  
							f++;
					
					}
					else next[r+1]--;
				}
			}
	
	
		}
		flag=next[r+1];
		for(int fz=0;fz<flag;fz++)
			for(int e=0;e<c2;e++)
				Candidate[fz][e]=Frequent[fz+f-flag][e];
	}////////////////////////////////////////////////////////////////////////////////////////////
	for(j=0;j<n;j++)
	{
		strcpy(temp[j],Candidate[j]);
	}
}

int main()
{	
	int i=0;
	int t=0;
	int ry;
	Apriori aprr;
	//aprr.print();
	char q1[c2]={'1','2','5','6','0'},q2[c2]={'1','3','4','7','0'},q3[c2][c2];
	char str1[c2]={'1','2','5','6','0'},str2[c2]={'6','1','0','0','0'};
	char cand[c2]={'1','3','0','0','0'};
	char Frq[25][c2];
	t=aprr.Join(q1,q2,q3);
	for(i=0;i<t+1;i++)
	{cout<<"\n";
		for(int j=0;j<c2;j++)
		cout<<q3[i][j];
	}
	cout<<"\n"<<t<<endl;
	ry=aprr.Substr(str1,str2);
	cout<<ry<<"yes!"<<endl;
	ry=aprr.Count_support(cand);
	cout<<ry<<" support count"<<endl;
	aprr.Associat(Frq);
	for(i=0;i<25;i++)
		for(int j=0;j<c2;j++)
			cout<<Frq[i][j];
			
			
	return 0;
}
