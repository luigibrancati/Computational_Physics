#include<iostream>
#include<fstream>
#include<cmath>
#include<typeinfo>
using namespace std;

typedef double M;

int factorial(int n){
	if(n==0) return 1;
	else n=n*factorial(n-1);
}

M serie_exp_factorial(M punto, int termini){
	M somma=0;
	for(int i=0; i<termini; i++) somma+=pow(-1,i)* pow(punto,i)/factorial(i);
	return somma;
}

int main(){
	cout << typeid(M).name() << endl;
	ofstream out;
	out.precision(15);
	int termini=0, punti=0;
	cout << "numero di termini della serie e dei punti\n";
	cin >> termini;
	cin >> punti;
	M * x=new M[punti];
	cout << "inserire i punti in cui calcolare exp\n";
	for(int i=0; i<punti; i++) cin >> x[i];
	
	out.open("trunc_round.txt", ios::out);
	if(out.is_open()){
		out << "#x\t\t" << "i\t\t" << "sum\t\t" << "exp\t\t" << "rel\t\t" << endl;
		for(int i=0; i<punti; i++){
			 out << x[i] << "\t\t" << termini << "\t\t" << serie_exp_factorial(x[i],termini) << "\t\t" << exp(-x[i]) << "\t\t" << abs(serie_exp_factorial(x[i],termini)-exp(-x[i]))/exp(-x[i]) << endl;
		}	
	}	
	else cout << "failed opening file\n";
	out.close();

	return 0;
}
