#include<iostream>
#include<fstream>
#include<cmath>
#include<cstdlib>
using namespace std;

template<typename T>
T absolute(T n){
	if(n>=0) return n;
	else return -n;
}

void corr_test(ofstream&);
float calculate_correlation(float*,int,int);
float* generate_array(int);

int main(){
	ofstream out("corr_test.txt");
	corr_test(out);	
	out.close();
	return 0;
}

void corr_test(ofstream& out){
	int k=0;
	cout << "test correlazione, inserire la potenza: ";cin>>k;cin.get();
	
	out <<"#correlazioni\n";
	float corr=0, delta=0;
	for(int i=10000; i<50000; i++){
		float * rng= generate_array(i);	
		corr=calculate_correlation(rng, i, k);
		delta=absolute(corr-1./4);
		out<<i<<" "<<delta<<endl;	
	}
}

float calculate_correlation(float * rng, int dim, int k){
	float corr=0;
	for(int i=0; i<(dim-k); i++) corr+=(rng[i]*rng[i+k])/dim;	
	return corr;
}

float * generate_array(int n){
	float * rng=new float[n];
	srand(time(NULL));
	for(int i=0; i<n; i++) rng[i]=((float)rand())/RAND_MAX; 
	
	return rng;	
}
