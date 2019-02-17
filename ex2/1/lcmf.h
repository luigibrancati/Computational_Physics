#ifndef LCMF_H_
#define LCMF_H_

#include<iostream>
#include<fstream>
using namespace std;

int determine_period(int * array, int dim){
	int * start1,* start2;
	int nu=0, period=0, mu=0;
	start1=array;
	start2=array;
	
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

void lcmf(int old, int terms, int m, int a, int c, int * serie){
	int x;
	
	serie[0]=old;	
	ofstream out("linear_congruent_method.txt");
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

#endif
