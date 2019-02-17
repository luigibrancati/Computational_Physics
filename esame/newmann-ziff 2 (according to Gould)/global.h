/*
	Algoritmo di Newmann-Ziff per la percolazione. Il lattice è considerato come un array di dimensione N, invece che una matrice L*L. Vedi ch.12 del Gould
	Da usare per misurare i dati
*/
#ifndef PERCOLATION
#define PERCOLATION

#include<iostream>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<iomanip>
#include<limits>
#include<cstring>
using namespace std;

#define W 20
const int EMPTY=numeric_limits<int>::min(); //assegno ai siti vuoti il valore minimo del tipo int

//funzione utile per dopo
template<typename T>
T sqr(T x){
	return x*x;
}

//classe che identifica la griglia
class latt{
	int L;		//dimensione lineare del lattice
	int N;		//siti totali, N=L*L
	int spanningsize;	//dimensione dello spanning cluster, 0 se non ce ne sono, assume al massimo uno spanning cluster
	long long int secondmoment; 	//secondo momento=somma{s^2*n_s}, s dimensione e n_s numero di cluster con dimensione s, serve per la dimensione media dei cluster
	int * order;		//ordine di occupazione dei siti	
	int * parent;		//array che serve per assegnare i siti ad un cluster e per riconoscere le root
	bool * touchleft;	//true se il sito alla posizione s tocca il bordo sinistro
	bool * touchright;	//true se il sito alla posizione s tocca il bordo destro

	void setorder(); 
	int findroot(int);
	int mergeclusters(int, int);
	int neighbour(int, int);
public:
	latt(int); //costruttore
	~latt(); //distruttore
	
	int numoccupied;	//siti occupati	
	int * clustdistrib;	//distribuzione, numero di cluster n_s di dimensione s
	
	void print();
	void initialize();
	void occupy_site();
	double meansize();

	//ritorna la dimensione dello spanning cluster
	int getspanningclustersize(){
		return spanningsize;
	}
};

//costruttore
latt::latt(int a)
{
	L=a;
	N=L*L;
	clustdistrib=new int[N+1];
	order=new int[N];
	parent=new int[N];
	touchleft=new bool[N];
	touchright=new bool[N];
}

//distruttore
latt::~latt(){
	delete[] clustdistrib;
	delete[] order;
	delete[] parent;
	delete[] touchleft;
	delete[] touchright;	
}

//print per controllare e stampare le posizioni dei siti con i rispettivi colori
void latt::print(){	
	ofstream out("lattice.txt", ios::app);
	out << "\n\n";
	int color;
	if(out.is_open()){
		for(int i=0; i<N; i++){
			if(parent[i]==EMPTY) color=0;
			else if(parent[i]<0) color=1;
			else color=(parent[i])%8 + 2;
			out << i%L << "\t" << setw(W) << i/L << "\t" << setw(W) << color << endl; 
		}
	}	
	else{
		cout << "error! file not open!\n";
		return;
	}
	out.close();
}

//genera una permutazione randomica degli indici dei siti, imposta l'ordine di occupazione nell'array order[]
void latt::setorder(){
	for(int i=0; i<N; i++){
		order[i]=i;
	}
		
	int r;
	for(int i=0; i<N-1; i++){
		r=rand()%(N-i) + i;
		swap(order[i], order[r]);
	}
}

//inizializza le quantitità
void latt::initialize(){
	int seed;
	cout << "seed: ";
	cin >> seed;
	srand(seed);
	
	numoccupied=0;
	spanningsize=0;
	secondmoment=0;

	setorder();
	clustdistrib[0]=0;
	for(int i=0; i<N; i++){
		clustdistrib[i+1]=0;
		parent[i]=EMPTY;	
		touchleft[i]=(i%L==0);
		touchright[i]=(i%L==L-1);		
	}
}

//dato un sito s, trova la root del cluster cui appartiene
int latt::findroot(int s){
	if(parent[s]<0) return s;
	else{
		parent[s]=findroot(parent[s]);
	}
	return parent[s];
}

//ritorna la posizione del vicino del sito s in direzione j, usa le open boundaries conditions
int latt::neighbour(int s, int j){
		switch(j){
		case 0: //destra
			return (s%L==L-1)? EMPTY : s+1;
		case 1://sotto
			return (s/L==0)? EMPTY : s-L;
		case 2://sinistra
			return (s%L==0)? EMPTY : s-1;
		case 3://sopra				
			return (s/L==L-1)? EMPTY : s+L;	
		default:
			return EMPTY;
		}
}

//fonde due cluster data la posizione delle loro root
int latt::mergeclusters(int r1, int r2){
	if(r1==r2) return r1;	
	else if(-parent[r1]<-parent[r2]){
		return mergeclusters(r2, r1);
  	}	
	else{
		clustdistrib[-parent[r1]]-=1;
		clustdistrib[-parent[r2]]-=1;
		clustdistrib[-parent[r1]-parent[r2]]+=1;
		secondmoment=secondmoment + sqr(parent[r1]+parent[r2])-sqr(parent[r1])-sqr(parent[r2]);
		//se uno dei due cluster tocca destra o sinistra, il nuovo cluster di radice r1 pure		
		touchleft[r1]=(touchleft[r1] || touchleft[r2]);
		touchright[r1]=(touchright[r1] || touchright[r2]);
		
		parent[r1]+=parent[r2];
		parent[r2]=r1;
		
		if(touchleft[r1] && touchright[r1]){
			spanningsize=-parent[r1];
		}
	}
	
	return r1;		
}

//occupa il sito successivo in order[] e controlla i vicini per controllare che non debba essere unito ad un altro cluster
void latt::occupy_site(){
	if(numoccupied==N){
		cout << "all occupied: " << numoccupied << endl;	
		return;
	}
	
	int pos=order[numoccupied++];
	parent[pos]=-1;//i siti occupati e non in un cluster valgono -1
	clustdistrib[1]+=1;
	secondmoment+=1;
	
	int rootnew=pos;
	int neighpos=0;
	for(int d=0; d<4; d++){
		neighpos=neighbour(pos, d);	
		if(neighpos!=EMPTY && parent[neighpos]!=EMPTY){
			rootnew = mergeclusters(rootnew, findroot(neighpos));
		}
	}	
}

//ritorna la dimensione media dei cluster per una data configurazione
double latt::meansize(){
	int spansize=getspanningclustersize();
	//correggo togliendo il cluster che spanna l'intera griglia
	long long int secondmomentcorrect=secondmoment-spansize*spansize;
	int occupiedcorrect=numoccupied-spansize;
	if(occupiedcorrect>0){
		return (double) secondmomentcorrect/occupiedcorrect;
	}
	else return 0;
}
#endif
