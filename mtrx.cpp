#include <iostream>
#include <conio.h>
#include <string>
#include <sstream>
#include <math.h>
#include "mtrx.h"

using namespace std;

#define MAXSIZE 21    /*  Maximum der Matrix  */
#define AZERO  1.0e-30

matrix::matrix(int zeilen, int spalten)
{
	int i,j;
	dim_y=zeilen;
	dim_x=spalten;

	elem=new Complex*[dim_x];
	for (i=0;i<dim_y;i++)
		elem[i]=new Complex[dim_y];

	for(i=0; i<dim_y; i++)
	{
		for(j=0; j<dim_x; j++)
			elem[i][j].Clear();
	}
}
matrix::matrix(const matrix &quelle)
{
	int i,j;
	dim_y=quelle.dim_y;
	dim_x=quelle.dim_x;

	elem=new Complex*[dim_x];
	for (i=0;i<dim_y;i++)
		elem[i]=new Complex[dim_y];

	for(i=0; i<dim_y; i++)
	{
		for(j=0; j<dim_x; j++)
			elem[i][j]=quelle.elem[i][j];
	}
}
/*matrix::~matrix()
{  
	if(dim_y>=dim_x)
	{
		for (int i=0;i<dim_y;i++) 
			delete elem[i];  
	}
	else
	{
	}
}*/ 
void matrix::StoreMatrix(string temp, int z, int s)
{
	this->elem[z-1][s-1].StrToComp(temp);
}
Complex matrix::GetComp(int i, int j)
{
	return(this->elem[i][j]);
}
void matrix::SetComp(int z, int s, Complex C)
{
	this->elem[z-1][s-1].StoreComp(C);
}
matrix matrix::operator *(matrix m2)
{
	matrix m_mul(this->dim_y,m2.dim_x);
	Complex test;
	test.SetRe(2);
	test.SetIm(9);
	for(int i=0; i<m_mul.dim_y; i++)
	{
		for(int j=0; j<m_mul.dim_x; j++)		{
			for(int k=0; k<this->dim_x; k++)
			{
				m_mul.elem[i][j]=this->elem[i][k]*m2.elem[k][j]+m_mul.elem[i][j];
			}
			//cout << endl << "M_mul(" << i << "," << j << "): " << m_mul.GetComp(i,j);
		}
	}
	return(m_mul);
}
/*
void gauss(double a[MAXSIZE][MAXSIZE], int n, double b[MAXSIZE],
		   int& flag, double x[MAXSIZE])
{
	double pivot,mult,temp;
	int i,j,k;
	
	flag=1;			// flag = 1  ->  Ergebnis gefunden
	
	//  Eliminations-phase     
	
	for (k=1;k<=n-1;k++)
	{
		pivot=a[k][k];
		if ( fabs(pivot) < AZERO )
		{
			flag=0;	// flag = 0  ->  das Pivotelement ist 0			
			break;
		}
		for (i=k+1;i<=n;i++)
		{
			mult=a[i][k]/pivot ;
			for (j=k+1;j<=n;j++)
				a[i][j]=a[i][j]-a[k][j]*mult;
			b[i]=b[i]-b[k]*mult;
		}
	}
	
	//    Zurück-substitution   
	
	flag=(flag) &&  (fabs(a[n][n] > AZERO));
	if (flag)
	{
		x[n]=b[n]/a[n][n];
		for (k=n-1;k>=1;k--)
		{
			temp=b[k];
			for(j=k+1;j<=n;j++)
				temp=temp-a[k][j]*x[j];
			x[k]=temp/a[k][k];
		}
	}
}*/