#include "vctr.h"
//#include "cmplx.h"
#include <iostream>
using namespace std;

/*int Vector::getlen() const
{
	return dim;
}


/*void vector::init(const double *const pd)
{
	for (int i=0;i<dim;++i)
		v[i]=pd[i];
}*/


Vector::Vector(int n)
{
	dim=n;
	elem=new Complex[dim];
	for (int i=0;i<n;i++)
		elem[i]=0.0;
}



//vector::vector(int n, double d ...)  {  if (n<=0) cout <<"vector: illegal dimension";  v=new double[len=n];  if (v==0) cout << "vector: out of memory";  v[0]=d; } vector::vector(int n, const double *pd)  {  if (n<=0) cout << "vector: illegal dimension";  v=new double[len=n];  if (v==0) cout << "vector: out of memory";  init(pd);  } vector::vector(const vector& w)  {  v=new double[w.len];  if (v==0) cout << "vector: out of memory";  for (int i=0;i<len;++i) v[i]=w.v[i];  }


Complex Vector::GetElem(int i)
{
	return elem[i];
}



void Vector::SetElem(int i, Complex C)
{
	elem[i].SetRe(C.GetRe());
	elem[i].SetIm(C.GetIm());
}


Vector Vector::operator &(Vector V2)
{
	Vector v_erg(this->dim);
	for(int i=0; i<dim;i++)
	{
		if(i<=dim-3)
			v_erg.elem[i]=this->elem[i+1]*V2.elem[i+2]-this->elem[i+2]*V2.elem[i+1];
		else
		{
			if(i==dim-2)
				v_erg.elem[i]=this->elem[dim-1]*V2.elem[0]-this->elem[0]*V2.elem[dim-1];
			if(i==dim-1)
				v_erg.elem[i]=this->elem[0]*V2.elem[1]-this->elem[1]*V2.elem[0];
		}
	}
	return v_erg;
}

Complex Vector::operator *(Vector V2)
{
	Complex skalar;
	for(int i=0; i<dim; i++)
		skalar+=this->elem[i]*V2.elem[i];
	return skalar;
}

Vector Vector::operator =(Vector V2)
{
	for(int i=0; i<dim;i++)
	{
		this->elem[i]=V2.elem[i];
	}
	return (*this);
}

/*void vector::clear()
{
	for (int i=0;i<dim;++i)
		v[i]=0.0;
}*/


/*void vector::print(void) const
{
	cout << "( "; for (int i=0;i<dim;++i) cout << v[i] << ' '; cout << ')';
} */
