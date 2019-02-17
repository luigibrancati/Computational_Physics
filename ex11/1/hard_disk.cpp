#include "global.h"

int main(){
	time_t t=clock();

	ofstream part_pos("particles_positions.txt");

	//inizializzo e accumulo g(r) sul primo stato
	starting();
	print_particles_positions(sistem, part_pos, 0);
	
	//equilibro il sistema facendolo evolvere per il 20% degli stati
	thermalize();
	
	//faccio evolvere e accumulo g(r), printo le posizioni ogni 50 passi temporali
	MCHD(part_pos);
	
	cout << "accepted: " << (double) accept/(N*nmcs) << endl;
	part_pos.close();

	char save;
	do{
		cout << "save last configuration? (y/n)\n"; cin >> save;
		if (save=='y'){
			save_config();//salva la configurazione finale
			break;
		}
		else if(save=='n') break;	
		else cout << "error! choose one between y/n\n";
	}while(true);

	t=clock();
	cout << "time: " << (double) t/CLOCKS_PER_SEC << endl;
}
