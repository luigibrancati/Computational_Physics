#include<iostream>
#include<fstream>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<iomanip>
using namespace std;

#define W 10

int nmcs=10000, accept, walkers=300;
double a, delta, Etot, Etotvar;//a è il parametro, delta è per lo step di montecarlo

vector<double> walk;

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
		walk[i]=((double) rand()/RAND_MAX)+0.5; 	
	}
}

void mcs(int w){
	double rtrial=0, p=0, s=0, E=0;
	do{
		rtrial=delta*(2*((double) rand()/RAND_MAX)-1) + walk[w];
	}while(rtrial<=0.01);
	p=exp(-2*a*(rtrial-walk[w]))*rtrial*rtrial;
	p=p/walk[w]/walk[w];
	s=(double) rand()/RAND_MAX;	
	if(s<p){
		walk[w]=rtrial;
		accept+=1;
	}
	
	E=-a*a + (2*a-2)/walk[w];
	Etot+=E;
	Etotvar+=E*E;
}

void mcs_total(){
	for(int i=0; i<walkers; i++){
		mcs(i);
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
	}

	Etot=Etot/(nmcs*walkers);
	Etotvar=Etotvar/(nmcs*walkers) - Etot*Etot;

	//out << "\n\n#final quantities\n#E_m\t" << setw(W) << "Evar\n" << "a: " << a << endl;
	cout << a << "\t" << setw(W) << Etot << "\t" << setw(W) << (double) sqrt(Etotvar/(nmcs*walkers)) << endl;	
	//cout << "\naccept: " << (double) accept/(nmcs*walkers) << endl;
	//out.close();
}

