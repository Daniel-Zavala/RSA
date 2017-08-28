#include<iostream>
#include<sstream>
#include<fstream>//files
#include<algorithm>//reverse
#include<stdlib.h>//random
#include<time.h>//srand

using namespace std;

//Converts integer value to 32 bit binary string
string toBinary(long long x){
	stringstream ssx;//to convert integer to string
	string sBin="";
	
	for(int i=0;i<32;i++){
		ssx<<x%2;
		x=x/2;
	}
	
	ssx>>sBin;
	reverse(sBin.begin(),sBin.end());//inverts string
	return sBin;
}

//modular exponentiation algorithm from unit textbook, x^y mod p
long long ModPowerX(long long x, long long y, long long p)
{
	long long c=0, f=1;
	string b="";
	
	b=toBinary(y);
	
	for(int i=0;i<b.length();i++)
	{
		c=c*2;
		f=(f*f)%p;
		if(b[i]=='1')
		{
			c=c+1;
			f=(f*x)%p;
		}
	}
	return f;
}

//extended euclidean algorithm, gcd(a mod b), x and y inverses, necesary to find 'd'
long long gcdExtended(long long a, long long b, long long *x, long long *y)
{
    // Base Case
    if (a == 0)
    {
        *x = 0;
        *y = 1;
        return b;
    }
 
    long long x1, y1; // To store results of recursive call
    long long gcd = gcdExtended(b%a, a, &x1, &y1);
 
    // Update x and y using results of recursive call
    *x = y1 - (b/a) * x1;
    *y = x1;
 
    return gcd;
}

//Lehmann's algorithm to verify if a number is prime or not, returns boolean
bool lehmann(long long p)
{
	long long a,aux;
	long long b=(p-1)/2;
	bool check=true;
	
	for(int i=2;i<200;i++)
	{
		a=i;
		aux=ModPowerX(a,b,p);
		if(!(aux == 1 || aux == p-1))
		{
			check=false;
		}
	}
	
	return check;
}

long long getP()
{
	long long p;
	p=rand() % 10000 + 1000;//get random number between 1000 and 10000
	while(!lehmann(p))//check if it is prime, if not, get another random number
	{
		p=rand() % 10000 + 1000;
		while(p%2==0)//discard even numbers
		{
			p=rand() % 10000 + 1000;
		}
	}
	return p;
}
long long getQ(long long p)
{
	long long q;
	q=rand() % 10000 + 1000;//get random number between 1000 and 10000
	while(!lehmann(q) && p!=q)//check if it is prime and different from p,
	{                         //if not, get another random number
		q=rand() % 10000 + 1000;
		while(q%2==0)//discard even numbers
		{
			q=rand() % 10000 + 1000;
		}
	}
	return q;
}
long long getE(long long fi)
{
	long long e,g,h;
	e=rand() % (fi-1) + 2;//get random number between 2 and fi-1
	while(gcdExtended(e,fi,&g,&h) != 1)//check if gcd(e,fi) = 1
	{	
		e=rand() % (fi-1) + 2;
	}
	return e;
}
long long getD(long long e, long long fi)
{
	long long x,y,d;
	d = gcdExtended(e,fi,&x,&y);//d must satisfy: (d*e) mod fi = 1
	d = x;//x is inverse
	return d;
}

//RSA Encryption c = m^e mod n
long long RSAENC(long long m, long long e, long long n)
{
	long long c;
	c=ModPowerX(m,e,n);
	return c;
}
//RSA Decryption m = c^d mod n
long long RSADEC(long long c, long long d, long long n)
{
	long long m;
	m=ModPowerX(c,d,n);
	return m;
}

int main()
{
	srand(time(NULL));//random seed
	
	long long p,q,n,e,d,fi;
	long long lAux,asdf,lfinal;
	string sAux="",sMessage="",sCipher="",sFinal="";
	unsigned char c;
	
	ifstream iFile;
	ofstream oEnc,oDec;
	
	//Selecting p value
	p=getP();
	cout<<"p = "<<p<<endl;
	
	//Selecting q value
	q=getQ(p);
	cout<<"q = "<<q<<endl;
	
	//get n and fi values
	n = p * q;
	fi = (p-1) * (q-1);
	cout<<"n = "<<n<<endl<<"fi = "<<fi<<endl;
	
	//Selecting e value
	e=getE(fi);
	cout<<"e = "<<e<<endl;
	
	//get d
	d = getD(e,fi);
	
	iFile.open("RSATest.txt");
	oEnc.open("RSAEncrypted.txt");
	oDec.open("RSADecrypted.txt");
	
	//read whole file
	while(!iFile.eof())
	{
		getline(iFile,sMessage);
		
		for(int i=0;i<sMessage.length();i++)
		{
			stringstream ss,ss2;
			
			c=sMessage[i];
			lAux = c;
			//////////////////////ENCRYPTION
			asdf=RSAENC(lAux,e,n);
			
			c=asdf;
			ss << c;
			string str = ss.str();
			oEnc << str;
			////////////////////////////////
			
			//////////////////////DECRYPTION
			lfinal=RSADEC(asdf,d,n);
			
			c=lfinal;
			ss2 << c;
			string str2 = ss2.str();
			oDec << str2;
			////////////////////////////////
		}
		if(!iFile.eof())
		{		
			oEnc << endl;
			oDec << endl;
		}
		
	}
	
	//close all files
	iFile.close();
	oEnc.close();
	oDec.close();

	cout<<endl<<"~~DONE~~"<<endl;
	
	system("PAUSE");
	return 0;
}
/* ModPowerX first digit
	for(int i=0;i<b.length();i++)
	{
		if(b[i]=='1')
		{
			first=i;
			break;
		}
	}*/
