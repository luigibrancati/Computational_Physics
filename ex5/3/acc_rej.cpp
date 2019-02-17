#include<iostream>
#include<fstream>
#include<cmath>
#include<iomanip>
#include<cstdlib>
using namespace std;

class func{
public:
	double operator()(double x){return (double) sqrt(1-x*x);}
};

double acc_rej_est(int);

int main(){
	int n_in, n_fin, n_steps;
	cout<<"initial points: "; cin>>n_in;
	cout<<"final points: "; cin>>n_fin;
	cout<<"steps: "; cin>>n_steps;

	double F_N;
	ofstream out("acc_rej.csv");
	out << setw(10)<<"#n\t"<<setw(10)<<"Pie\t"<<setw(10)<<"F_N\t"<<setw(10)<<"error\n";
	for(int i=n_in; i<=n_fin; i+=n_steps){
		F_N=acc_rej_est(i);
		out << setw(10)<< i << "\t"<<setw(10)<<M_PI<< "\t"<<setw(10)<<F_N<< "\t"<<setw(10)<<abs(M_PI-4*F_N)<<endl; 

	}
	out.close();

	return 0;
}

double acc_rej_est(int n_t){
	int n_i=0;
	double x, y;
	func f;

	for(int i=0; i<n_t; i++){
		x=(double) rand()/RAND_MAX;
		y=(double) rand()/RAND_MAX;

		if (y<f(x)) n_i+=1;
	}

	return (double) n_i/n_t;
}
