#include "global.h"

int L, N, numtrial=1;

double p_ns=0; //probabilità a cui calcolare la distribuzione media dei cluster! Vedi gourd
vector<double> meanclustersize;
vector<double> clustdistrib;
vector<double> p_inf; 
vector<double> p_span;

void onetrial();
void print_quantities();

int main(){
	time_t t=clock();
	cout << "L: ";		
	cin >>L;
	cout << L << endl;	
	N=L*L;	
		
	meanclustersize.resize(N,0);
	clustdistrib.resize(N+1,0);
	p_inf.resize(N,0);
	p_span.resize(N,0);
	
	for(int i=0; i<numtrial; i++){
		onetrial();
	}
	//print_quantities();

	t=clock();
	cout << "time: " << (double) t/CLOCKS_PER_SEC << endl;	

}

void print_quantities(){
	double p=0;
	ofstream out("quantities.txt");
	out << setw(W) << "p\t" << setw(W) << "Mean size\t" << setw(W) << "P inf\t" << setw(W) << "P span" << endl; 
	for(int i=0; i<N; i++){
		p=(double) (i+1)/N;
		meanclustersize[i]=(double) meanclustersize[i]/numtrial;
		p_inf[i]= (double) p_inf[i]/numtrial;
		p_span[i]=(double) p_span[i]/numtrial;
		clustdistrib[i+1]=(double) clustdistrib[i+1]/numtrial;
		out << setw(W) <<  p << "\t" << setw(W) << meanclustersize[i] << "\t" << setw(W) << p_inf[i] << "\t" << setw(W) << p_span[i];
		if(clustdistrib[i+1]>0) out << "\t" << setw(W) << i+1 << "\t"<< setw(W) << clustdistrib[i+1] << endl;
		else out << endl; 
	}
	out.close();
}

void onetrial(){
	latt lattice(L);
	lattice.initialize();
	
	for(int i=0; i<N; i++){
		lattice.occupy_site();
		lattice.print();
	}
}
