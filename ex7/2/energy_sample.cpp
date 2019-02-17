#include "/home/luigi/Fisica_computazionale/histogram.h"
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<iomanip>
using namespace std;

#define W 10
#define sigma 1

class func{
	double sig;
public:
	func(double x): sig(x) {cout << "generato\n";}
	double operator()(double x){return exp(-(x*x)/(2*sig*sig));}
};

double gaussian2();
double metropolis(double, double, func&);

int main(){
	int n=10000;//passi
	double x0=0, x, x1, x2, x3, x4, delta=5, ek, ep, et; //energie derivate da box-muller(bm) e da metropolis (ma)
	func f(sigma);

	srand(time(NULL));

	//box_muller
	ofstream out1("box_muller.csv");
	out1 << "#boxmuller\n";

	while(n<=100000){
		x=x0;	
		x1=x0; x2=pow(x0,2); x3=pow(x0,3); x4=pow(x0, 4);
		ek=0; ep=0;

		for(int i=0; i<n; i++){
			x=gaussian2()*sigma;
			x1+=x;
			x2+=pow(x, 2);
			x3+=pow(x, 3);
			x4+=pow(x, 4);
			ep+= 0.5*pow(x, 2);
			ek+=0.5*(1/(2*pow(sigma, 2)) - pow(x, 2)/(4*pow(sigma,4)));
		}
		
		out1 << n << "\t" << setw(W) << x1/n << "\t" << setw(W) << x2/n << "\t" << setw(W) << x3/n << "\t" << setw(W) << x4/n << "\t" << setw(W) << ep/n // 
		<< "\t" << setw(W) << abs(0.5*pow(sigma, 2) - ep/n)<< "\t" << setw(W) << ek/n << "\t" << setw(W) << abs(1/(8*pow(sigma, 2))-ek/n) << endl; 

		n+=1000;
	}
	out1.close();	

	//metropolis
	ofstream out2("metropolis.csv");
	out2 << "#metropolis\n";
	n=10000;

	while(n<=100000){
		x=x0;	
		x1=x0; x2=pow(x0,2); x3=pow(x0,3); x4=pow(x0, 4);
		ek=0; ep=0;	

		for(int i=0; i<n; i++){
			x=metropolis(x, delta, f);				
			x1+=x;
			x2+=pow(x, 2);
			x3+=pow(x, 3);
			x4+=pow(x, 4);
			ep+= 0.5*pow(x, 2);
			ek+=0.5*(1/(2*pow(sigma, 2)) - pow(x, 2)/(4*pow(sigma,4)));
		}
		
		out2 << n << "\t" << setw(W) << x1/n << "\t" << setw(W) << x2/n << "\t" << setw(W) << x3/n << "\t" << setw(W) << x4/n << "\t" << setw(W) << ep/n // 
		<< "\t" << setw(W) << abs(0.5*pow(sigma, 2)-ep/n)<< "\t" << setw(W) << ek/n << "\t" << setw(W) << abs(1/(8*pow(sigma, 2))-ek/n) << endl; 
		
		n+=1000;	
	}
	out2.close();
	
	return 0;
}

//algoritmo di box muller per dist. gaussiana
double gaussian2(){
	static bool todo2=true;
	static double redo2;
	double x1, y1, r2, x, y;

	if(todo2){
		do{
			x1=(2*((double)rand()/RAND_MAX)-1);
			y1=(2*((double)rand()/RAND_MAX)-1);
			r2=x1*x1+y1*y1;
		}while(r2>=1 || r2==0);

		x=x1*sqrt(-2*log(r2)/r2);				
		y=y1*sqrt(-2*log(r2)/r2);

		redo2=y;
		todo2=false;
		return x;
	}
	else{
		todo2=true;	
		return redo2;	
	}
}

double metropolis(double x0, double delta, func& f){
	double passo=2*delta*((double) rand()/RAND_MAX) - delta;	
	double x_trial=x0 + passo;
	
	
	double w=(double) f(x_trial)/f(x0);


	if(w>=1) return x_trial;
	else{
		double r=(double) rand()/RAND_MAX;
		if(r<=w){
			return x_trial;
		}
		else return x0;
	}	
}



