#include<iostream>
#include<cmath>
#include<typeinfo>
#include<fstream>
#include<iomanip>
using namespace std;

typedef float M;
const int w=18;

M ps(M (*f)(M), M h, M x){
	return (f(x+h)-f(x-h))/(2*h);
}

M pf(M (*f)(M), M h, M x){
	return (f(x+h)-f(x))/h ;
}

M pb(M (*f)(M), M h, M x){
	return (f(x)-f(x-h))/h;
}

int main(){
	M xf=1;
	M h[3]={0.5,0.2,0.1};
	ofstream out("derivative.txt");
	if(out.is_open()){	
		out.precision(6);
		out.flags(ios::left);
		out <<setw(w)<<"h"<<setw(w)<<"pf"<<setw(w)<<"pf err"<<setw(w)<<"pb"<<setw(w)<<"pb err"<<setw(w)<<"ps"<<setw(w)<<"ps error"<<setw(w)<<"cos"<< endl;
		for(int j=0; j<5; j++){
			M ht[3];
			for (int i=0; i<3; i++){ 
				ht[i]=h[i]/pow(10,j);
				out<<setw(w)<<ht[i]//
				<<setw(w)<<pf(sin,ht[i],xf)<<setw(w)<<pf(sin,ht[i],xf)-cos(xf)//
				<<setw(w)<<pb(sin,ht[i],xf)<<setw(w)<<pb(sin,ht[i],xf)-cos(xf)//
				<<setw(w)<<ps(sin,ht[i],xf)<<setw(w)<<ps(sin,ht[i],xf)-cos(xf)//
				<<setw(w)<<cos(xf)<< endl;
			}
	
		}
		out.close();
	}
	else cout << "unable to open file\n";
	return 0;
}
