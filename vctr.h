#include <iostream>
#include "cmplx.h"


using namespace std;

class Vector
{
private:
	int dim;
	Complex *elem;
public:
	Vector(int n=1);
	int getlen() const;
	Complex GetElem(int);
	void SetElem(int,Complex);
	void clear();
	void init(const double *const);
	void print() const;
	Vector operator &(Vector V2);
	Complex operator *(Vector V2);
	Vector operator =(Vector V2);
};