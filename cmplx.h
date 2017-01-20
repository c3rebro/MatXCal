#include <string>

using namespace std;

class Complex
{
private:
	double re, im;

public:
	Complex(double r=0.0, double i=0.0);
	void StrToComp(string);
	int CompLen();
	double GetRe();
	double GetIm();
	void StoreComp(Complex);
	void SetRe(double);
	void SetIm(double);
	void Clear();
	Complex operator +(Complex);
	Complex operator -(Complex);
	Complex operator *(Complex);
	Complex operator =(Complex);
	Complex operator +=(Complex);

	template<class T> T from_string(const string& Parameter)
	{
		T Ergebnis;
		stringstream tmp;

		tmp << Parameter;
		tmp >> Ergebnis;
		return Ergebnis;
	}

	template<class T> string to_string(const T& Parameter)
	{
		ostringstream tmp;

		tmp << Parameter;
		return tmp.str();
	}

	friend ostream &operator <<(ostream &, Complex &);
};