#include "global_LJ.h"

int main(){
	time_t t=clock();

	ofstream part_pos("particles_positions.txt");
	ofstream En("energy.txt");

	starting();
	print_particles_positions(sistem, part_pos, 0);	
	
	//termalizzo
	thermalize();
	
	//evolve
	MCLJ(En, part_pos);
	
	cout << "accepted: " << (double) accept/(N*nmcs) << endl;
	part_pos.close();
	En.close();

	t=clock();
	cout << "time: " << (double) t/CLOCKS_PER_SEC << endl;
}
