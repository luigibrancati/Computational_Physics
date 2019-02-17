#include "global.h"

int main() {	
	time_t t=clock();

	ofstream out_p("particle_pos.txt");
	out_p << "#x\t" << setw(W) << "y" << endl;
	ofstream out_q("quantities.txt");
	out_q << "t\t" << setw(W) << "dx mean\t" << setw(W) << "dy mean\t" << setw(W) << "dR2 mean\t" << setw(W) << "D(t)\t" << setw(W) << "D mean partial\t" << setw(W) << "D var partial"<< endl;
	ofstream out_back("back.txt", ios::app);

	starting_conditions();
	print_positions(out_p, 0);
	
	D_mean=0; D_var=0;
	for(int t=1; t<nmcs; t++){
		dx_m=0; dy_m=0; dR2=0; D_t=0;

		for(int i=0; i<N_p; i++) if(!full[i]) back[i]=dR[i];

		for(int w=0; w<N_p; w++){
			mc_step(t);
		}
		print_positions(out_p, t);

		dx_m=(double)dx_m/N_p;
		dy_m=(double) dy_m/N_p;		
		dR2=(double)dR2/N_p;
		D_t=a*a*(1./(4*t)) * dR2;
		D_mean+=D_t;
		D_var+=D_t*D_t;

		out_q << t << "\t" << setw(W) << dx_m << "\t" << setw(W) << dy_m << "\t" << setw(W) << dR2 << "\t" << setw(W) << D_t //
		      << "\t" << setw(W) << (double)D_mean/t << "\t" << setw(W) << ((D_var/t - (D_mean*D_mean)/(t*t))/t) <<  endl;

		
		for(int i=0; i<N_p; i++){
			if(full[i]){
				if(back[i]==dR[i]) back_count+=1;
				full[i]=false;
			}
			else if(back[i]!=dR[i]) full[i]=true;
		}
		
	}	

	out_back << p << "\t" << setw(W) << (double)nfail/(nmcs*N_p) << endl;
	
	out_back.close();
	out_q.close();
	out_p.close();

	D_mean=D_mean/nmcs;
	D_var=D_var/nmcs-D_mean*D_mean;
	ofstream out1("D_vs_p.txt", ios::app);
	out1 << p << "\t" << setw(W) << N_p << "\t" << setw(W) << L << "\t" << setw(W) << D_mean << "\t"  << setw(W) << D_var/nmcs << endl; 	
	out1.close();
	

	t=clock();
	cout << "t: " << (double) t/CLOCKS_PER_SEC << endl;
	return 0;
}


