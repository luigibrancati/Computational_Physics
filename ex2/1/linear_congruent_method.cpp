#include<iostream>
#include<cmath>
#include<fstream>
using namespace std;

int determine_period1(int*, int);
int determine_period2(int*, int);

int main(){
	int x, a=4, c=1, old, m=9, terms;
	cout << "inserire di seguito i valori richiesti\n";
	cout << "seed: "; cin >> old;
	cout << "termini: "; cin >> terms; 
	cout << "m: "; cin >> m;
	cout << "a: "; cin >> a;
	cout << "c: "; cin >> c;
	
	int * serie=new int[terms];
	serie[0]=old;
	ofstream out("linear_congruent_method.txt", ios::out);
	if(out.is_open()){
		out <<"#seed: "<<old<<" module= "<<m<<" a= "<<a<<" c= "<<c<<" numeri= "<<terms<<endl; 
		out << old << " ";
		for(int i=1; i<terms; i++){
			x=(a*old+c)%m;
			serie[i]=x;
			old=x;
			out << x <<" ";
		}
	out << endl;
	out.close();
	cout <<"periodo1= "<< determine_period1(serie, terms)<< endl;
	cout <<"periodo2= "<< determine_period2(serie, terms)<<endl;
	}
	else cout << "failure opening output file\n";
	
	delete[] serie;	
	return 0;
}

int determine_period1(int * array, int dim){//algoritmo visto su internet, hare and tortoise
	int * start1,* start2;
	int nu=0, period=0, mu=0;
	start1=array;
	start2=array;
	cout << "start: " << *start1 << endl;
	
	do{
		++start1; 
		start2+=2;
		nu+=1;
		if(nu==dim){
			cout << "no period was found\n";
			return 0;
		}
	}while(*start1!=*start2);

	start1=array;
	while(*start1!=*start2){
		++start1; ++start2;
		mu+=1;
	}

	start2=start1;
	do{
		++start2;
		period+=1;
	}while(*start1!=*start2);
	return period;
}

int determine_period2(int * array, int dim){//alg. diverso
	int period=0;
	int * start1,* start2;
	
	start1=array+(dim-1);
	start2=start1;
	cout << "last1:"<<*start1<<endl<<"last2:"<<*start2<<endl;
	
	for(int i=dim-1; i>=0; i--){
		--start2;
		++period;
		if(*start1==*start2) return period;
	}

	cout <<"no period was found\n";
	return 0;
}







