#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include<ctime>
#include<cstdlib>
using namespace std;

#define W 10

template<typename T>
T abs(T n){
	if(n>=0) return n;
	else return -n;
}

class logisticmap{
	double r;
public:
	logisticmap() {r=0;}
	logisticmap(double a, double b): r(a){}
	void setr(double a) {r=a;}
	void ask(){
		cout << "r:"; 
		cin>>r;
	}
	double getr(){return r;}
	double operator()(double x){return 4*r*x*(1-x);}
}f;

int nmax;
double xstart;
vector<double> x;

void starting(){
	f.ask();
	cout << "x0:"; cin >> xstart;
	cout << "nmax:"; cin >> nmax;
	x.resize(nmax,0);
}

void values(double x0){
	x[0]=x0;
	ofstream out("logisticmap.txt");
	for(int i=1; i<nmax; i++) {
		x[i]=f(x[i-1]);
		out << i << "\t" << setw(W) << x[i] << endl;
	}
	out.close();
}

void exponent(){
	double x1, x2, delta=0.1; 
	vector<double> difference;
	difference.resize(nmax, 0);
	for(int i=0; i<nmax; i++) cout << difference[i] << endl;
	starting();
	x1=xstart;

	ofstream out("exponent.txt");
		x2=x1+delta;
		values(x1);
		for(int i=0; i<nmax; i++) difference[i]=x[i];
		values(x2);
		for(int i=0; i<nmax; i++){
			difference[i]-=x[i];
			difference[i]=abs(difference[i]);
		}		
		for(int i=0; i<nmax; i++) out << (double) difference[i]/delta << endl;
	out.close();
}
