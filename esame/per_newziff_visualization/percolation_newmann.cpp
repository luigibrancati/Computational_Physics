/*
	Algoritmo di Newmann-Ziff per la percolazione, dal Gould.
	Da usare per la visualizzazione, immagini, gif ecc.
*/

#include "global.h"

int L, N, num=1000;

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

	cout << sizeof(unsigned long long int) << " " <<numeric_limits<unsigned long long int>::max() << endl;
	
	for(int i=0; i<num; i++){	
		onetrial();
	}	

	t=clock();
	cout << "time: " << (double) t/CLOCKS_PER_SEC << endl;	

}

void onetrial(){
	double size;
	latt lattice(L);
	lattice.initialize();
	
	for(int i=0; i<N; i++){
		lattice.occupy_site();
		lattice.meansize();
		//lattice.print();				
	}
}
