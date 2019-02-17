#include<iostream>
#include<fstream>
#include<cmath>
#include<iomanip>
#include<string>
#include<vector>
using namespace std;

#define W 10

int N;
vector<double> Dist;

void binomial(double p){
	int nmax=round(p*N);		
	double normalization=0;
	Dist.resize(N+1,0);
	Dist[nmax]=1;

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

	string filename="binom"+to_string(N)+".txt";
	ofstream out(filename);
	for(int i=0; i<=N; i++){
		Dist[i]=(double) Dist[i]/normalization;
		out << i << "\t" << setw(W) << Dist[i] << endl;
	}
	out.close();
}

int main(){
	double p=0.5;

	cout << "N:"; cin >> N;

	binomial(p);

}
