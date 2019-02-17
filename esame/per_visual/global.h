//NON E' NEWMANN ZIFF

#include<iostream>
#include<fstream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<iomanip>
#include<vector>
using namespace std;

#define W 10

vector<int> lattice;
int L, N, clustnumber;
double p;

void print(){
	ofstream out("lattice.txt");
	for(int i=0; i<N; i++){
		out << i%L << "\t" << setw(W) << i/L << "\t" << setw(W) << lattice[i] << endl;
	}
	out.close();
}

void starting(){
	int seed=533707444;
	srand(seed);
	cout << seed << endl;
	//cout << "L: "; 
	//cin >> L;
	L=50;
	//cout << "p: "; 
	cin >> p;
	N=L*L;
	clustnumber=0;

	lattice.resize(N, -2);
}

//open boundaires
int neightbour(int s, int j){
	switch(j){
	case 0://destra
		return (s%L==L-1)? -1 : s+1;
	case 1://sotto
		return (s/L==0)? -1 : s-L;
	case 2://sinistra
		return (s%L==L-1)? -1 : s-1;
	case 3://sopra
		return (s/L==L-1)? -1 : s+L;
	}
}

void occupy(){
	double r;
	for(int i=0; i<N; i++){
		r=(double) rand()/RAND_MAX;
		if(r<p){
			lattice[i]=-1;
		}		
	}
}

void cluster(int s){
		lattice[s]=clustnumber;
		int nb;
		for(int i=0; i<4; i++){
			nb=neightbour(s, i);
			if(nb>=0 && lattice[nb]==-1){
				lattice[nb]=lattice[s];				
				cluster(nb);
			}
		}	
}

void clusters(){
	for(int i=0; i<N; i++){
		if(lattice[i]==-1){		
			cluster(i);
			clustnumber=(clustnumber+1)%8;
		}
	}
}


