#include<iostream>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<cstdlib>
using namespace std;

class func{
public:
	double operator()(double x) {return exp(-x*x);}
};

class dist{
public:
	double operator()(double a, double x) {return a*exp(-x);}
};

template<typename T>
T abs(T n){
	if (n>=0) return n;
	else (-n);
}

void sample_mean(int, func&, double&, double&);
void importance_sample(int, func&, dist&, double&, double&);

int main(){
	func f;
	dist p;
	double F_N_sm, F_N_is, sigma_sm, sigma_is;
	int n_min, n_max, n_steps;
	cout<<"inserire n min: "; cin>>n_min;
	cout<<"inserire n max: "; cin>>n_max;
	cout<<"inserire n steps: "; cin>>n_steps;

	const double int_val=sqrt(M_PI)/2 * erf(1);
	ofstream out("sm_is.csv");
	out<<setw(10)<<"#n\t"<<setw(10)<<"int\t"<<setw(10)<<"F_N_sm\t"<<setw(10)<<"sigma_sm\t"<<setw(10)<<"sigma_sm/n\t"<<setw(10)<<"F_N_is\t"<<setw(10)<<"sigma_is\t"<<setw(10)<<"sigma_is/n"<<endl;
	for(int i=n_min; i<=n_max; i+=n_steps){
		sample_mean(i, f, F_N_sm, sigma_sm);
		importance_sample(i, f, p, F_N_is, sigma_is);			
	
		out<<setw(10)<<i<<"\t"<<setw(10)<<int_val<<"\t"<<setw(10)<<F_N_sm<<"\t"<<setw(10)<<sigma_sm<<"\t"<<setw(10)<<(double) sigma_sm/sqrt(i)//
		   <<"\t"<<setw(10)<<F_N_is<<"\t"<<setw(10)<<sigma_is<<"\t"<<setw(10)<<sigma_is/sqrt(i)<<endl;
	}	
	out.close();

	return 0;
}

void sample_mean(int n, func& f, double& F_N, double& sigma){
	double x;
	
	for(int i=0; i<n; i++){
		x=(double) rand()/RAND_MAX;
		F_N+=f(x);
		sigma+=f(x)*f(x);
	}
	
	F_N=(double) F_N/n;
	sigma=sqrt(((double) sigma/n)-F_N*F_N);
}

void importance_sample(int n, func& f, dist& p, double& F_N, double& sigma){
	double x, y;
	double A=exp(1)/(exp(1)-1);	
	
	for(int i=0; i<n; i++){
		x= (double) rand()/RAND_MAX;
		y=-log(x*(1-1/exp(1)) + 1/exp(1));	
		F_N+=f(y)/p(A,y);
		sigma+=(f(y)*f(y))/(p(A,y)*p(A,y));
	}	

	F_N=(double) F_N/n;
	sigma=sqrt(((double) sigma/n)-F_N*F_N);		
}

