#include<iostream>
#include<fstream>
#include<cmath>
#include<cstdlib>
#include<iomanip>
#include<ctime>
using namespace std;

#define W 10

void microstates(int, ofstream&);

int main(){
	int N;
	cout << "N: "; cin >> N;
		
	srand(time(NULL));
	ofstream out("microstates.txt", ios::app);
	microstates(N, out);
	out.close();
		
	return 0;
}

//genera i macrostati con nleft
void microstates(int N, ofstream& out){
	int nl=N, tmax=50*N, n_runs=1;
	double prob, r, nmean, nvar;

	double * P_n=new double[N+1];
	for(int i=0; i<N+1; i++) P_n[i]=0;
	double * n_mean=new double[tmax];	
	for(int i=0; i<tmax; i++) n_mean[i]=0;	

	out << "#t\t" << setw(W) << "nl\t" << setw(W) << "prob\n";

	ofstream out1("n_mean.txt");

	n_mean[0]=N;
	for(int j=0; j<n_runs; j++){		
		nl=N;		
		
		out << "\n\n#N=" << N << endl;
		out << "#run: " << j << endl; 
		out << 0 << "\t" << setw(W) << nl << "\t" << setw(W) << 1 << endl;
		
		P_n[nl]+=1; nmean=nl; nvar=nl*nl;
		for(int i=1; i<tmax; i++){
			prob=(double)nl/N;
			r=(double)rand()/RAND_MAX;

			if(r <= prob) nl-=1;
			else nl+=1;
			
			if (i>75) {
				nmean+=nl;
				nvar+=nl*nl;
			}

			P_n[nl]+=1;

			out << i << "\t" << setw(W) << nl << "\t" << setw(W) << prob << endl; 
			n_mean[i]+=nl;
		}

		nmean=nmean/tmax;
		nvar=nvar/tmax - nmean*nmean;
		cout << nmean << endl << sqrt(nvar/tmax) << endl;
	}

	ofstream out2("histogram.txt", ios::app);
	out2 << "#N= " << N << endl;
	for(int i=0; i<N+1; i++) out2 << i << "\t" << setw(W) << P_n[i]/tmax << endl;
	out2.close();	

	out1 << "\n#t\t" << setw(W) << "n mean\n";
	out1 << 0 << "\t" << setw(W) << N << endl;
	for(int i=1; i<tmax; i++){
		n_mean[i]=n_mean[i]/n_runs;
		out1 << i << "\t" << setw(W) << n_mean[i] << endl;
	}

}
