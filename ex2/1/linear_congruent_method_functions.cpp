#include<iostream>
#include<cmath>
#include<fstream>
using namespace std;

void linear_congruen_method(){
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
	cout <<"periodo= "<< determine_period(serie, terms)<< endl;
	}
	else{
		 cout << "failure opening output file\n";
		return;
	}
}	

int determine_period(int * array, int dim){
	int * start1,* start2;
	int nu=0, period=0, mu=0;
	start1=array;
	start2=array;
	cout << "start: " << *start1 << endl;
	
	do{
		++start1; cout <<"1:"<<*start1 << endl;
		++start2; ++start2; cout <<"2:"<<*start2 << endl;
		nu+=1;
		if(nu==dim){
			cout << "no period was found\n";
			return 0;
		}
	}while(*start1!=*start2);

	start1=array;
	cout <<"passaggio 2\n";
	while(*start1!=*start2){
		++start1; cout<<"1;"<<*start1<<endl; ++start2;cout<<"2:"<<*start2<<endl;
		mu+=1;
	}

	start2=start1;
	cout <<"passaggio 3\n";
	do{
		++start2; cout<<"2:"<<*start2<<endl;
		period+=1;
	}while(*start1!=*start2);
	return period;
}
