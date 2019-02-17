#include<iostream>
#include<fstream>
#include<cstdlib>
#include<time.h>
#include<cmath>
using namespace std;

template<typename T>
T absolute(T num){
	if(num>=0) return num;
	else return -num;
}

void deviations_test(ofstream&);
float * generate_array(int);
float mean_moment(float*,int,int);

int main(){

	ofstream out2("deviation.txt");
	deviations_test(out2);	
	out2.close();
	
	return 0;
}

void deviations_test(ofstream& out){
	int k=0;
	cout<<"test di uniformità, inserire la potenza: "; cin>>k;
		
	float xk=0;	
	out << "#deviazioni\n";
	
	
	for(int i=1000; i<50000; i++){		
		float * rng=generate_array(i);	
		xk=mean_moment(rng, i, k);	
		float dev=absolute(xk-1./(k+1));
		out << i << " " << dev << endl;
	}
}

float * generate_array(int n){
	float * rng=new float[n];
	srand(time(NULL));
	for(int i=0; i<n; i++) rng[i]=((float)rand())/RAND_MAX; 
	
	return rng;	
}

float mean_moment(float * array, int dim,int power){
	float xkm=0;
	for(int i=0; i<dim; i++) xkm+=pow(array[i],power)/dim;
	return xkm;
}


