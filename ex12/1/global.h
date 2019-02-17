#include<iostream>
#include<fstream>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<iomanip>
using namespace std;

#define W 10

int nmcs=100000, accept, walkers=300;
double a, delta, Etot, Etotvar;//a è il parametro, delta è per lo step di montecarlo

vector<double> walk;

template<typename T>
T abs(T n){
	if(n>=0) return n;
	else return -n;
}

class func{
protected:
	double beta;
public: 
	func(double b): beta(b) {}
	virtual double operator()(double) =0;
};

class parabula: public func
{
public:
	parabula(double b): func(b) {}
	double operator()(double x){
		if(abs(x)<beta) return (beta*beta-x*x);
		else return 0;
	}
};

class exponential: public func
{
public: 
	exponential(double b): func(b) {}
	double operator()(double x){return exp(-2*x*x*beta);}
};

void initialize(){
	time_t seed=time(NULL);
	srand(seed);
	//cout << seed << endl;	

	//cout << "a: "; 
	cin >> a;
	delta=0.1;

	accept=0;
	walk.resize(walkers, 0);
	for(int i=0; i<walkers; i++){
		walk[i]=((double) rand()/RAND_MAX) - 0.5;	
	}
}

void mcs(){
	double xtrial, r, w, p, pn, pd, E;
	w=rand()%walkers; //randomizzo il walker da muovere
	xtrial=delta*(2*((double) rand()/RAND_MAX)-1) + walk[w];
	r=(double) rand()/RAND_MAX;

	pd=a*a-walk[w]*walk[w];
	if(abs(xtrial)>=a) p=0;
	else{
		pn=a*a-xtrial*xtrial;
		p=(pn/pd)*(pn/pd);
	}

	if(r<p){
		walk[w]=xtrial;
		accept+=1;
	}
	
	E=walk[w]*walk[w]*0.5 + 1/(a*a-walk[w]*walk[w]);
	Etot+=E;
	Etotvar+=E*E;
}

void mcs_total(){
	for(int i=0; i<walkers; i++){
		mcs();
	}
}

void thermalize(){
	int steps=0.2*nmcs;
	int eqsteps=0.1*steps+1;
	for(int i=0; i<steps; i++){
		mcs_total();
		if((i+1)%eqsteps==0){
			delta*=accept/(0.5*walkers*eqsteps);
			accept=0;
		}
	}

	accept=0;
	//cout << "delta: " << delta << endl;
}

void vmc(){
	//ofstream out("wfc_exp.txt");
		
	//accumulation
	Etot=0;
	Etotvar=0;	
	for(int i=0; i<nmcs; i++){
		mcs_total();
		//cout << i << "\t" << setw(W) << Etot << "\t" << setw(W) << Etotvar << endl;
	}

	Etot=Etot/(nmcs*walkers);
	Etotvar=Etotvar/(nmcs*walkers) - Etot*Etot;

	//cout << "\n\n#final quantities\n#E_m\t" << setw(W) << "Evar\n" << "a: " << a << endl;
	cout << a << "\t" << setw(W) << Etot << "\t" << setw(W) << (double) sqrt(Etotvar/(nmcs*walkers)) << endl;	
	//cout << "\naccept: " << (double) accept/(nmcs*walkers) << endl;
	//out.close();
}

