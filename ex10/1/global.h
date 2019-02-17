#ifndef GLOBAL_LATTICE_H_
#define GLOBAL_LATTICE_H_

#include <iostream>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<cmath>
using namespace std;

#define W 10

//Side sites (side lenght), number of particles, monte carlo steps, equilibrium step; back_mov is used to count how many times a particle goes back to it's previous position at t+1 after moving at t
int seed, L, N_p, nmcs=10000, nfail=0; 
//lenght of lattice step, density, time interval per step, mean x displacement, mean y disp., mean squasre disp., D, D mean and D variance;
double a=1, p, dt=1, dx_m, dy_m, dR2, D_t, D_mean, D_var;
int back_count=0;//aiuta per controllare se le particelle tornano con back
//lattice, array with positions of all particles at each instant, displacement of each particle from start (calculated at each time step and at the end total)
vector<vector<bool> > lattice;
vector<vector<int> > particle_positions;
vector<vector<int> > dR;
vector<vector<int> > back; //controlla se le particelle tornano o meno alla posizione iniziale
vector<bool> full; //aiuta per il back

//print positions of particles on lattice at each time step on file
void print_positions(ofstream& out, int step){
	out << ((step>0)? "\n\n#step: " : "#step: ") << step << endl;
	for(int i=0; i<N_p; i++){
		out << particle_positions[i][0] << "\t" << setw(W) << particle_positions[i][1] << endl;
	}
}

//allow one to choose wether to imput values of N and L by hand, or to choose a value for density and pick one among the displayed allowed integer values (nov=number of values) of N and L that give same density
void p_N_L(){
	int choose, nov=8;	
	cout << "do you want to:\n0. define N and L indipendently\n1. given p, choose between N and L allowed (first " << nov << " results)\nchoose: "; cin >> choose;
	
	if(choose==0){	
		cout << "L: "; cin >> L;
		cout << "N_p: "; cin >> N_p;
		p=(double) N_p/(L*L);
	}
	else{
		int count=0, N_trial=0, L_trial_int=0;		
		double L_trial=0;
		vector<vector<double> > values(nov, vector<double>(2,0));
		cout << "p: "; cin >> p;
		
		do{
			N_trial+=1;
			L_trial=sqrt((double)N_trial/p);
			L_trial_int=L_trial;

			if(L_trial==L_trial_int){
				values[count][0]=N_trial; values[count][1]=L_trial;
			 	cout << count << ". N: " << N_trial << "\tL: " << L_trial << endl;
				++count;
			}
			
		}while(count<nov);

		cout << "choose: "; cin >> choose;
		N_p=values[choose][0]; L=values[choose][1];
		cout << "\nchoosen: N: " << N_p << "\tL: " << L << endl; 
	}
}

//generate starting conditions: L, N number of part., density and seed, also generate lattice and the other array and place particles randomly as a starting state
void starting_conditions(){
	
	seed=time(NULL);
	srand(seed);
	cout << "seed: " << seed << endl;

	p_N_L();
	
	lattice.resize(L, vector<bool>(L,false));
	particle_positions.resize(N_p, vector<int>(2,0));
	dR.resize(N_p, vector<int>(2,0));
	back.resize(N_p, vector<int>(2,0));
	full.resize(N_p, false);

	int x, y;
	for(int i=0; i<N_p; i++){
		while(true){
			x=rand()%L;
			y=rand()%L;

			if(!lattice[y][x]){
				lattice[y][x]=true;
				particle_positions[i][0]=y;
				particle_positions[i][1]=x;
				break;
			}
		}
	}
}

//single mc step for one time evolution
void mc_step(int step){
	int particle, direction, dx=0, dy=0, px, py, px_trial, py_trial;

	particle=rand()%N_p; //particle to move
	py=particle_positions[particle][0];//randomized particle position
	px=particle_positions[particle][1];
	px_trial=px; 
	py_trial=py;
	direction=rand()%4; //randomized direction to move: 0 est, 1 sud, 2 ovest, 3 nord
	
	//update particle position based on moving direction using pbc
	switch(direction){
	case 0://nord
		if(py==0) py_trial=L-1;
		else py_trial=py-1;
		dy=-1;
		break;
	case 1://est
		if(px==L-1) px_trial=0;
		else px_trial=px+1;
		dx=+1;		
		break;
	case 2://sud
		if(py==L-1) py_trial=0;
		else py_trial=py+1;
		dy=+1;		
		break;
	case 3://ovest
		if(px==0) px_trial=L-1;
		else px_trial=px-1;
		dx=-1;		
		break;
	}

	if(!lattice[py_trial][px_trial]){
		lattice[py_trial][px_trial]=true;
		lattice[py][px]=false;
		particle_positions[particle][0]=py_trial;
		particle_positions[particle][1]=px_trial;
		
		dR[particle][0]+=dy; dR[particle][1]+=dx;
		dx_m+=dR[particle][0]; dy_m+=dR[particle][1];
		dR2+=dR[particle][0]*dR[particle][0]+dR[particle][1]*dR[particle][1];

		
	}
	else nfail+=1;
}




#endif
