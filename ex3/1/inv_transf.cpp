#include<iostream>
#include<fstream>
#include<cmath>
#include<cstdlib>
using namespace std;

template<typename T>
T max(T* array, int dim){
	T max=0;
	for(int i=0; i<dim; i++) if(array[i]>max) max=array[i];
	return max;
}

double lambda;

double * generate_array(int);
void histogram1(double *, int);

//exponential distribution
int main(){
	int n=0;
	cout<<"inserire lambda\n"; cin>>lambda;
	cout <<"lunghezza sequenza: "; cin >> n;	

	double * x=generate_array(n);

	double * z=new double[n];	
	ofstream out("inv_transf.txt");
	for(int i=0; i<n; i++){
		z[i]=-log(x[i]);
		out<< z[i] <<endl;	
	}	

	//histogram
	histogram1(z,n);
	
	out.close();
	return 0;
}

double * generate_array(int n){
	double * rng=new double[n];
	srand(time(NULL));
	for(int i=0; i<n; i++){
		do{
		rng[i]=((double)rand())/RAND_MAX; 
		}while(rng[i]==0);	
	}	
	return rng;	
}

void histogram1(double * rng, int n){
	int hist_int=0;
	cout<<"\nhistogram intervals: "; cin>>hist_int;
	
	double delta=2./hist_int/lambda;
	double checksum=0;
	int * h=new int[hist_int+1];	
	
	int bin;
	for(int i=0; i<n; i++){
		bin=rng[i]/delta/lambda + 1;			
		if (bin <= hist_int) h[bin]+=1;
	}

	ofstream out("histogram.txt");
	out<<"\n\n#histogram1\n";
	for(int i=0; i<hist_int; i++){
		out<<i*delta<<"\t"<<(double) h[i]/(n*delta)<<endl;
		checksum+=(double) h[i]/(n * delta);
	}
	out<<"\n#checksum="<<checksum<<endl;
	out.close();
}
