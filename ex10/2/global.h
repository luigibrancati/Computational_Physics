#include<iostream>
#include<fstream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<iomanip>
using namespace std;

#define W 10

int L, seed, nmcs=100000, walkers, mass;
double dr_max, sticking_prob=1;

vector<int> seed_pos(2,0);
vector<vector<bool> > lattice;
vector<vector<int> > cluster; 
vector<int> pos(2,0);

template<typename T>
T abs(T n){
	if (n>=0) return n;
	else return -n;
}

template<typename T>
double module(T a, T b){
	return (double) sqrt(a*a+b*b);
}

void starting(){
	seed=time(NULL);
	srand(seed);

	lattice.clear();
	lattice.resize(L, vector<bool>(L,false));
	seed_pos[0]=L/2;
	seed_pos[1]=L/2;
		
	cluster.clear();
	cluster.push_back(seed_pos);	//posizione del seed iniziale
	lattice[seed_pos[0]][seed_pos[1]]=true;
	mass=1;
	dr_max=5;
}

void calc_drmax(){
	double max=0, mod;
	for(int p=0; p<cluster.size(); p++){
		mod=module(cluster[p][0]-seed_pos[0], cluster[p][1]-seed_pos[1]);
		if(mod > max){
		 max=mod;
			//cout << cluster[p][1] << cluster[p][0] << endl;
		}		
	}
	dr_max=max+5;//maximum displacement + 5 for maximum generation distance of walkers
}

void walker_evolution(){
	//starting angular position of walker, sticking probability check and module of position (for later)
	double theta, stick, mod;
	// direction, starting position and trial position for evolution. l is the step lenght: 1 is R<R_max, otherwise equals the distance of R from R_max minus 1, to get the walker inside R_max
	int dir, x_start, y_start, x_trial, y_trial, l;
	
	do{	
		theta=2*M_PI*((double) rand()/RAND_MAX); //angle of positioning
		x_start=round(dr_max*cos(theta)+seed_pos[1]);//intero più vicino al valore x e y di partenza
		y_start=round(dr_max*sin(theta)+seed_pos[0]);		
	
		if(!lattice[y_start][x_start]){
			pos[0]=y_start; 
			pos[1]=x_start;
			break;
		}
	}while(true);

	for(int i=1; i<nmcs; i++){
		
   		dir=rand()%4;
		y_trial=pos[0]; x_trial=pos[1];
		mod=module(pos[0]-seed_pos[0], pos[1]-seed_pos[1]);
		l=( (mod>dr_max)? round(mod-dr_max+1): 1);
		switch(dir){
		case 0://nord
			y_trial-=l;	
			break;
		case 1://est
			x_trial+=l;
			break;
		case 2://sud
			y_trial+=l;
			break;
		case 3://ovest
			x_trial-=l;			
			break;
		}

		stick=(double)rand()/RAND_MAX;
		if(!lattice[y_trial][x_trial]){
			pos[0]=y_trial;
			pos[1]=x_trial;
			if(module(pos[0]-seed_pos[0], pos[1]-seed_pos[1])> 2*dr_max) return; //kills the walker
		}
		else if(stick<sticking_prob){	
			lattice[pos[0]][pos[1]]=true;
			cluster.push_back(pos);
			calc_drmax();
			mass+=1;
			return;
			
		}		
	}
}

void verify_occupancy(){
	int runs=10000, sum=0;
	double dr_max_t;
	vector<double> occ_prob(6,0);
	ofstream out("occupation.txt");
	
	L=100;
	walkers=100;
	starting();
	lattice[seed_pos[0]+1][seed_pos[1]]=true;
	mass+=1;
	calc_drmax();
	dr_max_t=dr_max;

	for(int i=0; i<runs; i++){	

		//simulazioni
		do{			
			walker_evolution();
		}while(mass<3);

		if(lattice[seed_pos[0]+2][seed_pos[1]]) occ_prob[0]+=1;
		else if (lattice[seed_pos[0]+1][seed_pos[1]+1]) occ_prob[1]+=1;
		else if (lattice[seed_pos[0]][seed_pos[1]+1]) occ_prob[2]+=1;
		else if (lattice[seed_pos[0]-1][seed_pos[1]]) occ_prob[3]+=1;
		else if (lattice[seed_pos[0]][seed_pos[1]-1]) occ_prob[4]+=1;
		else if (lattice[seed_pos[0]+1][seed_pos[1]-1]) occ_prob[5]+=1;

		lattice[pos[0]][pos[1]]=false;
		cluster.pop_back();
		dr_max=dr_max_t;
		mass=2;
	}
	
	for(int i=0; i<6; i++){
		sum+=occ_prob[i];
		out << i << "\t" << setw(W) << (double) occ_prob[i]/runs << endl;
	}
	out << endl << sum << endl;
	out.close();
	
}

void print_mass(ofstream& out){
	out << dr_max-5 << "\t" << setw(W) << mass << endl;
}

void print_cluster(ofstream& out, int step){
	for(int i=0; i<cluster.size(); i++){
		out << i << "\t" << setw(W) << cluster[i][1] << "\t" << setw(W) << cluster[i][0] << endl;
		if(i%(walkers/10)==0) out << endl << endl;
	}	
}
