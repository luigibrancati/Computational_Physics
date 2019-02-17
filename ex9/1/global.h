#ifndef GLOBAL_H_
#define GLOBAL_H_

#include<iostream>
#include<fstream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<iomanip>
using namespace std;

#define W 10
enum spin {u=1,d=-1}; //spin values
vector<vector<spin> > lattice;

int seed, L, N, nmcs=10000, nequil=1000, accept; //lattice dimension, montecarlo steps and equilibration time
double T, E, M; //Temperature, energy and magnetization
double c, chi, E_mean, M_mean, E_var, M_var, M_abs; //cumulative quantities 

//stampa il lattice dividendo spin up da spin down su due file diversi
void print_lattice(ofstream& out_up, ofstream& out_down){
	out_up << "\n\n";
	out_down << "\n\n";
	for(int i=0; i<L; i++){
		for(int j=0; j<L; j++){
			if(lattice[i][j]==u){								
				out_up << j << "\t" << setw(W) << i << endl;
			}
			else{
				out_down << j << "\t" << setw(W) << i << endl;
			}
		}
	}
}

//calcola la variazione di energia per uno spin flip
//assumo PB conditions
double dE(int i, int j){
	double dE=0, coeff=0;
	
	//up and down
	if(i==0) coeff+=lattice[1][j]+lattice[L-1][j];
	else if(i==L-1) coeff+=lattice[0][j]+lattice[L-2][j];
	else coeff+=lattice[i-1][j]+lattice[i+1][j];

	//left and right
	if(j==0) coeff+=lattice[i][L-1]+lattice[i][1];
	else if(j==L-1) coeff+=lattice[i][L-2]+lattice[i][0];
	else coeff+=lattice[i][j-1]+lattice[i][j+1];
	
	dE=2*lattice[i][j]*coeff;
	return dE;
}

//accumulo per le quatità medie
void accumulate(){
	E_mean+=E;
	M_mean+=M;
	E_var+=E*E;
	M_var+=M*M;
	M_abs+=abs(M);
}


//step di montecarlo
void montecarlo_step(int step){
	int i, j;	
	double r, d_E;		
	
	for(int k=0; k<N; k++){
			i=rand()%L;
			j=rand()%L;
			r=(double)rand()/RAND_MAX;			
	
			d_E=dE(i, j);

			if(exp(-d_E/T) > r){
				lattice[i][j]=(spin) -lattice[i][j];
				
				accept+=1;	
				M+=2*lattice[i][j];
				E+=d_E;
			}
	}
	
	if(step>=nequil) accumulate();
	
}

//calcola la derivata di E_mean, cioè C
void c_der(ofstream& out){
	static double c_deriv=-E_mean;
	c_deriv+=E_mean;
	c_deriv=c_deriv/0.1;
	out << T-0.1 << "\t" << c_deriv/N << endl;
	c_deriv=-E_mean;
}

//inizializza il lattice ad una certa configurazione iniziale ed inzializza energia, magnetizzazione ecc. 
void starting_lattice(int scelta){

	seed=1496224546;
	srand(seed);
	cout << seed << endl;

	lattice.clear();
	lattice.resize(L, vector<spin>(L,u));	
	
	switch(scelta){
		case 1:
			break;
		case 2:
			for(int i=0; i<L; i++){
				for(int j=0; j<L; j++) lattice[i][j]=d;			
			}
			break;
		case 3:
			for(int i=0; i<L; i++){
				for(int j=0; j<(L/2); j++) {
					lattice[i][j]=u;
					lattice[i][j+L/2]=d;
				}			
			}
			break;
		case 4:
			
			for(int i=0; i<L/2; i++){
				for(int j=0; j<L; j++) {
					lattice[i][j]=u;
					lattice[i+L/2][j]=d;
				}			
			}		
			break;
		case 5:
			for(int i=0; i<L; i+=2){
				for(int j=0; j<L; j++) {
					lattice[i][j]=u;
					if(i<L-1) lattice[i+1][j]=d;
				}			
			}
			break;
		case 6:			
			for(int i=0; i<L; i++){
				for(int j=0; j<L; j+=2) {
					lattice[i][j]=u;
					if(j<L-1) lattice[i][j+1]=d;
				}			
			}
			break;
		case 7:
			lattice[0][0]=u;
			for(int i=0; i<L; i++){
				for(int j=0; j<L; j++){
					if((i+j)%2==0) lattice[i][j]=u;
					else lattice[i][j]=d;
				}
			}
			break;
		case 8:
			for(int i=0; i<L; i++){
				for(int j=0; j<L; j++){
				 	if(rand()%2==0) lattice[i][j]=d;
					else lattice[i][j]=u;
				}		
			}
			break;
	}	

	//inizialization of quantities	
	E=0; M=0; c=0; chi=0; E_mean=0; M_mean=0; E_var=0; M_var=0; M_abs=0; accept=0; 

	double E_coeff;
	for(int i=0; i<L; i++){
		for(int j=0; j<L; j++){
			M+=lattice[i][j];
			
			E_coeff=0;
			//PBC up
			if(i==0) E_coeff+=lattice[L-1][j];
			else E_coeff+=lattice[i-1][j];	
			
			//PBC right
			if(j==L-1) E_coeff+=lattice[i][0];
			else E_coeff+=lattice[i][j+1];	
			
			E-=lattice[i][j]*E_coeff;
		}
	}
	
	if(nequil==0){
		accumulate();
	}			
}

void ising_evolution(){
	ofstream out("quantities.txt");
	ofstream out_up("lattice_up.txt");	
	ofstream out_down("lattice_down.txt");
	out_up << "#colonna\t" << setw(W) << "riga\n";		
	out_down << "#colonna\t" << setw(W) << "riga\n";		
	
	//inizializzo la griglia di spin
	cout << "L: "; cin >> L;	
	cout << "T: "; cin >> T;
	N=L*L;
	int scelta;
	cout << "scegliere il tipo di lattice di partenza: \n 1. tutti up \n 2. tutti down \n 3. metà (verticale)" //
		 "\n 4. metà (orizzontale) \n 5. righe alternate \n 6. colonne alternate \n 7. alternati\n 8. random\n scelta:";
	cin >>scelta;
	starting_lattice(scelta);
	print_lattice(out_up, out_down);
	out << "#seed: " << seed << "\ni\t" << setw(W) << "E\t" << setw(W) << "M\t" << setw(W) << "E p_mean\t" << setw(W) << "M p_mean\n" << endl;
	out << 0 << "\t" << setw(W) << E/N << "\t" << setw(W) << M/N << "\t" << setw(W) << E_mean/N << "\t" << setw(W) << M_mean/N << endl;

	//il primo step è quello iniziale
	for(int i=1; i<nmcs; i++){
		montecarlo_step(i);
		if(i%50==0 && i<(nmcs-1)) print_lattice(out_up, out_down);
	
		out << i << "\t" << setw(W) << E/N << "\t" << setw(W) << M/N;
		if(i<nequil) out << endl; 
		else out << "\t" << setw(W) << E_mean/((i-nequil+1)*N)<< "\t" << setw(W) << M_mean/((i-nequil+1)*N) << endl;	
	}
	print_lattice(out_up, out_down);

	E_mean=E_mean/(nmcs-nequil);
	M_mean=M_mean/(nmcs-nequil);
	E_var=E_var/(nmcs-nequil)-E_mean*E_mean;
	M_var=M_var/(nmcs-nequil)-M_mean*M_mean;
	M_abs=M_abs/(nmcs-nequil);
	c=(1/(T*T))*(E_var);
	chi=(1/T) * (M_var);
	
	out << "\n\n#final mean quantities\n" << T << "\t" << setw(W) << (double)E_mean/N << "\t" << setw(W) << sqrt(E_var/(nmcs-nequil))/N << "\t" //
		     << setw(W) << (double)M_mean/N << "\t" << setw(W) << sqrt(M_var/(nmcs-nequil))/N << "\t" << setw(W)// 
       		     << (double)M_abs/N << "\t" << setw(W) << c/N << "\t" << setw(W) << chi/N << endl; 

	out_up.close();
	out_down.close();
	out.close();
}

void isingVStemperature(){
	ofstream out_mean("mean_quantities.txt");
	ofstream out_c_der("c_der.txt");
	out_mean << "#quantità medie\n" << "T\t" << setw(W) << "E mean\t" << setw(W) << "E var\t" << setw(W) << "M mean\t" << setw(W) << "M var\t" << setw(W) << "M abs\t" //
	     << setw(W) << "th. capac.\t" << setw(W) << "susc" << endl;  
	
	
	cout << "L: "; cin >> L;	
	cout << "T: "; cin >> T;
	N=L*L;
	int scelta;
	cout << "scegliere il tipo di lattice di partenza: \n 1. tutti up \n 2. tutti down \n 3. metà (verticale)" //
		 "\n 4. metà (orizzontale) \n 5. righe alternate \n 6. colonne alternate \n 7. alternati\n 8. random\n scelta:";
	cin >>scelta;
	for(T=2; T<=3; T+=0.1){
		starting_lattice(scelta);
		for(int i=1; i<nmcs; i++){
			montecarlo_step(i);
		}
			
		E_mean=E_mean/(nmcs-nequil);
		M_mean=M_mean/(nmcs-nequil);
		E_var=E_var/(nmcs-nequil)-E_mean*E_mean;
		M_var=M_var/(nmcs-nequil)-M_mean*M_mean;
		M_abs=M_abs/(nmcs-nequil);
		c=(1/(T*T))*(E_var);
		chi=(1/T) * (M_var);
	
		out_mean << T << "\t" << setw(W) << (double)E_mean/N << "\t" << setw(W) << sqrt(E_var/(nmcs-nequil))/N << "\t" //
		     << setw(W) << (double)M_mean/N << "\t" << setw(W) << sqrt(M_var/(nmcs-nequil))/N << "\t" << setw(W)// 
       		     << (double)M_abs/N << "\t" << setw(W) << c/N << "\t" << setw(W) << chi/N << endl; 

		c_der(out_c_der);
	}	
	
	out_c_der.close();
	out_mean.close();
}
#endif
