#include<iostream>
#include<fstream>
#include<cmath>
#include<cstdlib>
#include<string>
#include<vector>
using namespace std;

template<typename T>
T abs(T n){
	if(n>=0) return n;
	else return -n;
}

template<typename T>
T max(std::vector<T> array){
	T max=0;
	for(int i=0; i<array.size(); i++) if(array[i]>max) max=array[i];
	return max;
}

void histogram(std::vector<double>);
void alg1(int);
void alg2(int);

int main(){
	int n=0;
	cout<<"lunghezza serie: "; cin>>n;
	
	srand(time(NULL));	

	alg1(n);		

	alg2(n);

	return 0;
}

void histogram(std::vector<double> rng, int alg){
	int hist_int=0;
	cout<<"\nhistogram intervals: "; cin>>hist_int;
	
	double delta=2./hist_int;
	int checksum=0;
	int dim=rng.size();
	
	int * h=new int[hist_int+1];
	for(int i=0; i<=hist_int; i++) h[i]=0;
	
	for(int i=0; i<dim; i++){
		int bin= (rng[i]+1)/delta;
		if (bin<=hist_int) h[bin]+=1;
	}
	
	string filename="histogram"+ to_string(alg) + ".txt";
	ofstream out(filename);
	out<<"\n\n#histogram\n";
	for(int i=0; i<hist_int; i++){
		out<<i*delta - 1 <<"\t"<< (double) h[i]/dim/delta <<endl;
		checksum+=h[i];
	}
	out<<"\n#checksum="<<checksum<<endl;
	out.close();
}

void alg1(int dim){
	double x;
	int i=0;
	vector<double> z(dim,0);	

	ofstream out("alg1.txt");
	double temp=0;
	do{
		do{
			x=(double) rand()/RAND_MAX;		
			z[i]=sin(M_PI*(2*x-1));	
		}while(z[i]==1 && z[i]==-1);	
		out << z[i]<<endl;
		i++;
	}while(i < dim);

	histogram(z, 1);
	out.close();
}

void alg2(int dim){
	double u, v;
	int i=0;
	std::vector<double> x(dim,0);
		
	ofstream out("alg2.txt");
	do{
		u=(double) rand()/RAND_MAX;
		v=(double) rand()/RAND_MAX;			
		if(u*u+v*v < 1){
			x[i]=(double) (u*u-v*v)/(u*u+v*v);			
			out<< x[i] <<endl;
			i++;
		}
	}while(i < dim);

	histogram(x, 2);
	out.close();
}
