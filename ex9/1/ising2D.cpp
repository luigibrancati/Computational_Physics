#include "global_open.h"

int main(){
	clock_t t=clock();		
	
	ising_evolution();
	//isingVStemperature();	

	t=clock();
	cout << "time: " << ((double) t/CLOCKS_PER_SEC) << endl;

	return 0;
}


