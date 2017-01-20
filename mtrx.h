#include "vctr.h"

#define MAXSIZE 21    /*  Maximum der Matrix  */
#define AZERO  1.0e-30
class matrix : public Vector //Vererbung nur zu dem Zweck, dass Vector die Klasse Complex nutzen kann
{
protected:
	Complex **elem;
	int dim_x, dim_y;

public:
	matrix(int z=1, int s=1);
	matrix(const matrix &);
//	~matrix();
	Complex GetComp(int, int);
	void SetComp(int, int, Complex);
	matrix operator *(matrix);
	matrix operator <<(Complex);
	void StoreMatrix(string temp,int dim_y,int dim_x);
	void gauss(double a[MAXSIZE][MAXSIZE], int n, double b[MAXSIZE],int &flag, double x[MAXSIZE]);
};