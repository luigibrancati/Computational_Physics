#include<iostream>
#include<fstream>
#include<cstdlib>
#include<time.h>
using namespace std;

template<typename T>
void print_rng(T * rng, int dim, ofstream& out){
	for(int i=0; i<dim-1; i++) out<<rng[i]<<" "<<rng[i+1]<<endl;
}

float * generate_array(int);
void histogram(float, float*, int, ofstream&);

int main(){
	
	ofstream out("intrinsic.txt");
	if(out.is_open()){		
		int n=0;
		cout<<"numbers:"; cin>>n; cin.get();
	
		float * rng=generate_array(n);
		print_rng(rng,n,out);		

		float delta=0.1;
		histogram(delta, rng, n, out);				
	
		out.close();		
	}	
	else cout<<"file not opened\n";
	return 0;
}

float * generate_array(int n){
	float * rng=new float[n];
	srand(time(NULL));
	for(int i=0; i<n; i++) rng[i]=((float)rand())/RAND_MAX; 
	
	return rng;	
}

void histogram(float delta, float * rng, int n, ofstream& out){
	int nh=1/delta;
	int checksum=0;
	int * h=new int[nh];	
	
	for(int i=0; i<n; i++) h[(int)(rng[i]/delta)]+=1;
	
	out<<"\n\n#histogram\n";
	for(int i=0; i<nh; i++){
		out<<h[i]<<endl;
		checksum+=h[i];
	}
	out<<"\n#checksum="<<checksum<<endl;
}


