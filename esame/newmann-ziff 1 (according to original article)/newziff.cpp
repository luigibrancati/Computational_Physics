/*
	Algoritmo di Newmann-Ziff con la convoluzione. In questo file è contenuto il main con le funzioni necessarie ad eseguire l'algoritmo e accumulare le quantità, l'algoritmo in sé è contenuto
	nel file global.h. Questo algoritmo calcola soltanto la probabilità cui nella relazione ci si riferisce con R_L (p), eseguendo una convoluzione. 
	
	Per eseguire questo codice, basta mettere global.h e questo file nella stessa cartella e dare il comando
	g++ -std=c++11 percolazione.cpp -o perc.out
	
	Per eseguire più di una iterazione, basta modificare il valore di trials e preparare un file (es. input.txt) con il valore di L alla prima riga e i seed per ogni iterazione, uno per ogni riga, 
	e mandarlo in input con
	./perc.out < input.txt 
*/

#include "global.h"

//dimensione lineare della griglia, numero di siti e numero di iterazioni dell'algoritmo
int L, N, trials=1;

//quantità
vector<double> S;
vector<double> Dist;

void onetrial();
void binomial(double);

//main
int main(){
	time_t t=clock();

	cout << "L: ";
	cin >> L;
	N=L*L;

	//inizializzo S e la calcolo eseguendo onetrial()
	S.resize(N,0);
	for(int i=0; i<trials; i++){
		onetrial();
	}

	//normalizzo S(n)
	for(int i=0; i<N; i++){
		S[i]=(double) S[i]/trials;
	}
	
	//calcolo R_L (p)
	string filename="R"+to_string(L)+".txt";
	ofstream out(filename);

	double deltap=0.000001;
	double p=0.56;
	while(p<0.6){
		cout << p << endl;
		//calcolo la distribuzione binomiale alla p considerata
		binomial(p);		

		//calcolo il valore di R_L
		double value=0;
		for(int i=0; i<N; i++){
			value+=S[i]*Dist[i+1];
		}
		out << p << "\t" << setw(W) << value << endl;
		//nuova p		
		p+=deltap;
	}
	out.close();	

	//tempo
	t=clock();
	cout << "time:" << (double) t/CLOCKS_PER_SEC << endl;

	return 0;
}

//riempio la griglia e calcolo S(n)
void onetrial(){
	latt lattice(L);
	lattice.initialize();

	for(int i=0; i<N; i++){
		lattice.percolate();

		if(lattice.getspanningclustersize()!=0){
			//S(n)=0 per n<n* e S(n)=1 per n>n*			
			for(int j=i; j<N; j++){
				S[j]+=1;
			}
			break;
		}
	}
}

//calcolo la distribuzione binomiale
void binomial(double p){
	int nmax=round(p*N);		
	double normalization=0;
	Dist.resize(N+1,0);
	Dist[nmax]=1;

	//calcolo i valori con il metodo iterativo
	normalization+=Dist[nmax];
	for(int i=nmax-1; i>0; i--){
		double a=(double) (i+1)/(N-i);
		double b=(1-p)/p;
		Dist[i]=Dist[i+1]*a*b;
		normalization+=Dist[i];
	}
	for(int i=nmax+1; i<=N; i++){
		double a=(double) (N-i+1)/i;
		double b=p/(1-p);
		Dist[i]=Dist[i-1]*a*b;
		normalization+=Dist[i];
	}

	//normalizzo
	for(int i=0; i<=N; i++){
		Dist[i]=(double) Dist[i]/normalization;
	}
}

