#include<iostream>
#include<fstream>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<iomanip>
using namespace std;

#define W 10

double a=1;

class func{
protected:
	double beta;
public: 
	func(double b): beta(b) {cout << "func" << endl;}
	virtual double operator()(double) =0;
};

class parabula: public func
{
public:
	parabula(double b): func(b) {cout << "creata parabola!" << endl;}
	double operator()(double x){
		cout << "op della parabola" << endl;
		if(abs(x)<beta) return (beta*beta-x*x);
		else return 0;
	}
};

class exponential: public func
{
public: 
	exponential(double b): func(b) {cout << "creato exp!" << endl;}
	double operator()(double x){
		cout << "op dell'exp" << endl;	
		return exp(-2*x*x*beta);
	}
};

func * f;

int main(){
	a=0.6;
	exponential e(a);
	double p=exp(-2*a*(4-1));	
	double p1=e(2)/e(1);
	cout << p << " "  << p1<< endl;

}
