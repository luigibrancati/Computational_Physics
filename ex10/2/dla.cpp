#include "global.h"

int main(){
	time_t t=clock();	


	ofstream out_cluster("final_cluster.txt");
	out_cluster << "part\t" << setw(W) << "x\t" << setw(W) << "y" << endl;
	ofstream out_mass("mass_vs_r.txt", ios::app);

	//verify_occupancy();	
	
	
	cout << "walkers: "; cin >> walkers;
	cout << "L: "; cin >> L;
	starting();
	for(int w=1; w<walkers; w++){
		walker_evolution();
	}
	
	print_cluster(out_cluster, walkers);
	print_mass(out_mass);
	
	cout << "m: " <<  mass << endl;
		
	t=clock();
	cout << "t: " << (double)t/CLOCKS_PER_SEC << endl;
	return 0;
}
