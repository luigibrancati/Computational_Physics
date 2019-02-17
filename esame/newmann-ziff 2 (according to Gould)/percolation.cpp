/*
	Algoritmo di Newmann-Ziff. In questo file è contenuto il main con le funzioni necessarie ad eseguire l'algoritmo e accumulare le quantità, ma l'algoritmo di Newmann-ziff in sé è contenuto
	nel file global.h. Questo algoritmo calcola le varie quantità di interesse, ma si è esclusa la probabilità media  di comparsa dello spanning cluster. Per calcolare tale quantità, si aggiunga il
	seguente codice

	//fuori dal main, scope globale
	//eliminare le altre quantità
	long long int pfirst, pvar;

	
	//in alltrials()
	pfirst=0;
	pvar=0;
	//eliminare print_quantities() e le altre quantità	
	
	//in onetrial()
	for(int i=0; i<N; i++){
		lattice.occupy_site();

		//eliminare le altre quantità
		if(lattice.getspanningclustersize()!=0){
			pfirst+= (i+1);
			pvar+= (i+1)*(i+1);
			break; //interrompo il ciclo non appena ho lo spanning cluster		
		}
	}	
	
	Basta quindi stampare, in ordine
	
	pfirst= (double) pfirst/N/numtrial;
	pvar = (double) pvar/N/N/numtrial - pfirst*pfirst;



	Per eseguire questo codice (a prescindere da cosa si voglia calcolare), basta mettere global.h e questo file nella stessa cartella e dare il comando
	g++ -std=c++11 percolazione.cpp -o perc.out
	
	Per eseguire più di una iterazione, basta modificare numtrial e preparare un file (es. input.txt) con il valore di L alla prima riga e i seed per ogni iterazione, uno per ogni riga,
	e mandarlo in input con
	./perc.out < input.txt 
*/

#include "global.h"

//Dimensione lineare della griglia, numero di siti e iterazioni dell'algoritmo
int L, N, numtrial=1;

double p_ns=0.5; //probabilità a cui calcolare la distribuzione media dei cluster!

void alltrials();
void onetrial();

//vettori per accumulare le quantità ai vari valori di p e fare la media
vector<double> P_inf;
vector<double> Pspan;
vector<double> Pspanvar; //varianza di P_span
vector<double> S;
vector<double> distrib; //distribuzione della dimensione dei clusters


//main
int main(){
	time_t t=clock();
	
	alltrials();

	t=clock();
	//stampo a schermo il tempo impiegato
	cout << "time: " << (double) t/CLOCKS_PER_SEC << endl;	

}

//stampa le quantità
void print_quantities(){
	double p=0;
	//apro un file "quantitiesL.txt"
	string filename="quantities" + to_string(L) + ".txt";
	ofstream out(filename);
	
	out.precision(10);
	out << "#L=" << L << endl << setw(W) << "p\t" << setw(W) << "P span\t" << "Pspanvar\t" << setw(W) <<"P inf\t" << setw(W) << "S\t"<< setw(W) << "Dist" << endl; 
	for(int i=0; i<N; i++){
		p=(double) (i+1)/N;
		P_inf[i]=(double) P_inf[i]/numtrial;
		S[i]=(double) S[i]/numtrial;
		Pspan[i]=(double) Pspan[i]/numtrial;
		Pspanvar[i]= (double) Pspanvar[i]/numtrial - Pspan[i]*Pspan[i];

		out << setw(W) <<  p << "\t" << setw(W) << Pspan[i] << "\t" << setw(W) << sqrt(Pspanvar[i]/numtrial) << "\t" << setw(W) << P_inf[i] << "\t" << setw(W) << S[i];
		if(distrib[i+1]>=0) out << "\t" << setw(W) << i+1 << "\t" << setw(W) << (double) distrib[i+1]/numtrial/N << endl;
		else out << endl; 
	}
	out.close();
}

//eseguo una sola iterazione dell'algoritmo, parto da una griglia vuota e la riempio calcolando le quantità alle varie p
void onetrial(){
	latt lattice(L);
	lattice.initialize();
	
	for(int i=0; i<N; i++){
		lattice.occupy_site();

		S[i]+= lattice.meansize();		
		P_inf[i]+=(double) lattice.getspanningclustersize()/lattice.numoccupied;
		int p=(lattice.getspanningclustersize()==0? 0 : 1);		
		Pspan[i]+= p;
		Pspanvar[i]+= p*p;
		
		int k=round(p_ns*N);
		if(k==i){
			for(int j=0; j<=N; j++){
				distrib[j]+=lattice.clustdistrib[j];		
			}		
		}
	}
}

//eseguo l'algoritmo molte volte, tante quante indicate in numtrial, per accumulare le quantità e fare la media
void alltrials(){
	cout << "L: ";
	cin >> L;	
	N=L*L;	

	//inizializzo		
	P_inf.resize(N,0);
	S.resize(N,0);
	distrib.resize(N+1,0);
	Pspan.resize(N,0);
	Pspanvar.resize(N,0);
	
	//eseguo l'algoritmo
	for(int i=0; i<numtrial; i++){
		onetrial();
	}

	//stampo
	print_quantities();
}
