#include <iostream>
#include "mtrx.h"
using namespace std;

class qmatrix : public matrix
{
public:
	qmatrix(const int);
	Complex determinante(int);
};