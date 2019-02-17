#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cmath>
#include<iomanip>
#include<ctime>
using namespace std;

#define W 15

class func{
public:
	double operator()(double x){return ((x+0.2)*x+cos(14.5*x+0.3));}
};

int main(){
	func f;
	int nsteps=1000;
	bool check=true;
	double T, Tscale, scale=0.5, x_t, fx_t, x, fx, x_min, fx_min;
	cout << "T start: "; cin >> T;
	cout << "T scale: "; cin >> Tscale;
	cout <<	"x start: " ; cin >> x;
	
	x_min=x;
	fx=f(x);
	fx_min=fx;

	ofstream out("sim_annehal.txt");
	out << left;
	out.precision(15);
	out << "#T\t" << setw(W) << "i\t" << setw(W) << "x\t" << setw(W) << "fx\t" << setw(W) << "x min\t" << setw(W) << "fx min" << endl;
	while(T>1.e-05){
		for(int i=0; i<nsteps; i++){
			out << T <<"\t" << setw(W) << i << "\t" << setw(W) << x << "\t" << setw(W) << fx << "\t" << setw(W) << x_min << "\t" << setw(W) << fx_min << endl;			
			
			x_t=x+scale*sqrt(T)*((double)rand()/RAND_MAX-0.5);
			fx_t=f(x_t);						
			if(exp(-(fx_t-fx)/T)> ((double)rand()/RAND_MAX)){
				fx=fx_t;
				x=x_t;				
			}
			if(fx < fx_min){
				x_min=x;
				fx_min=fx;
			}
		}

		T=T*Tscale;
	}
	out.close();
	return 0;
}
