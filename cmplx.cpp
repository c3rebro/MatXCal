#include <iostream>
#include <string>
#include <sstream>
#include "cmplx.h"

using namespace std;

Complex::Complex(double r, double i)
{
	this->re=r;
	this->im=i;
}
void Complex::Clear()
{
	this->re=0;
	this->im=0;
}
void Complex::StrToComp(string inp)
{
	string input=inp.c_str();
	string temp, re_arr, im_arr;
	int x,o,n,pm=0,ro=0,mal=0;

	for(x=0; input[x]!='\0'; x++)
	{
		if((input[x]=='+') || (input[x]=='-')) ro=pm=x;
		if(input[x]=='*') mal=x;
	}
	
	if(pm==0)
	{
		temp=input;
		re=from_string<double>(temp);
	}
	else
	{
		for(n=mal+1, o=0; n<x; n++, o++);
		
		re_arr.resize(pm);
		im_arr.resize(o);
		pm--;
		
		for(; pm>=0; pm--)
			re_arr[pm]=input[pm];
		for(; n>mal; --o,--n)
			im_arr[o]=input[n];	

		temp=re_arr;
		this->re=from_string<double>(temp);
		
		temp=im_arr;
		this->im=from_string<double>(temp);

		if(input[ro]=='-')
			this->im=this->im*(-1);
	}
}

int Complex::CompLen()
{
	int len=1;

	if(this->im)
	{
		len+=strlen(to_string(this->im).c_str());
		len+=3;
	}

	if(this->re)
		len+=strlen(to_string(this->re).c_str());
	else
		len+=3;

	return(len);
}


double Complex::GetRe()
{
	return(this->re);
}

double Complex::GetIm()
{
	return(this->im);
}


void Complex::SetRe(double r)
{
	this->re=r;

}


void Complex::SetIm(double i)
{
	this->im=i;
}


void Complex::StoreComp(Complex C)
{
	this->re=C.re;
	this->im=C.im;
}

Complex Complex::operator +(Complex C2) 
{ 
	Complex Sum; 
	Sum.SetRe(this->re+C2.GetRe()); 
	Sum.SetIm(this->im+C2.GetIm()); 
	return (Sum); 
} 

Complex Complex::operator -(Complex C2) 
{ 
	Complex Diff;
	Diff.SetRe(this->re-C2.GetRe()); 
	Diff.SetIm(this->im-C2.GetIm()); 
	return (Diff);
}

Complex Complex::operator *(Complex C2)
{
	Complex Prod;
	Prod.re+=this->re*C2.re-this->im*C2.im;
	Prod.im+=this->re*C2.im+C2.re*this->im;
	return (Prod);
}

Complex Complex::operator =(Complex C2)
{
	this->re=C2.re;
	this->im=C2.im;
	return (*this);
}

Complex Complex::operator +=(Complex C2)
{
	this->re=C2.re+this->re;
	this->im=C2.im+this->im;
	return (*this);
}


ostream &operator <<(ostream &out, Complex &C)
{
	if(C.im==0)
		out << C.re;
	else
	{
		if(C.im<0)
		{
			C.im=-C.im;
			out << C.re << "-j*" << C.im;
		}
		else
			out << C.re<< "+j*" << C.im;
	}		
	return out;
}