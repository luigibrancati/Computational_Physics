#include<iostream>
#include<fstream>
#include<cmath>
#include<iomanip>
#include<cstdlib>
using namespace std;

#define W 10

template<typename T>
T max(T* n, int dim){
	T max=0;
	for(int i=0; i<dim; i++) if(n[i]>max) max=n[i];
	return max;
}


template<typename T>
T min(T* n, int dim){
	T min=0;
	for(int i=0; i<dim; i++) if(n[i]<min) min=n[i];
	return min;
}

void histogram(double *, int);
double gaussian2();
double mean(double*,int);
double variance(double*,int);

int main(){
	int n=0;
	cout <<"lunghezza serie: "; cin>>n;

	ofstream out("gaussian2.txt");
	double * x=new double[n];
	for(int i=0; i<n; i++){
		x[i]=gaussian2();
		out<<x[i]<<endl;
	}

	out<<"\n\n#mean= " << mean(x,n) <<endl;
	out<<"#variance= "<< variance(x,n) <<endl;
	out.close();

	histogram(x, n);

	return 0;
}

double mean(double * rng, int dim){
	double mean_v=0;
	double sum=0;

	for(int i=0; i<dim; i++)sum+=rng[i];

	return (double)(sum/dim);
}

double variance(double * rng, int dim){
	double variance_v=0;
	double mean_v=mean(rng, dim);
	double sum=0;	
	
	for(int i=0; i<dim; i++) sum+=(rng[i]-mean_v)*(rng[i]-mean_v);
	
	return (double)(sum/dim);
}

void histogram(double * rng, int dim){
	int nh=0;
	cout<<"inserire numero intervalli: "; cin >> nh;	
	
	double xmax=max(rng, dim);
	double xmin=min(rng, dim);
	double delta=(double)(xmax-xmin)/nh;	
	int checksum=0;
	
	double * h=new double[nh];
	for(int i=0; i<nh; i++) h[i]=0;

	if(xmin<0) for(int i=0; i<dim; i++) rng[i]-=xmin;

	for(int i=0; i<dim; i++) h[int(rng[i]/delta)]+=1;
	
	ofstream out("histogrampolar.txt");
	out << "\n\n#histogram\n";
	for(int i=0; i<nh; i++){
		out<< setw(W) << (double) i*delta+xmin << "\t" << setw(W) << (double) h[i]/(dim*delta) <<endl;
		checksum+=h[i];
	}
	out<<"\n#checksum= "<<checksum<<endl;
	out.close();
}

double gaussian2(){
	static bool todo2=true;
	static double redo2;
	double x1, y1, r2, x, y;

	if(todo2){
		do{
			x1=(2*((double)rand()/RAND_MAX)-1);
			y1=(2*((double)rand()/RAND_MAX)-1);
			r2=x1*x1+y1*y1;
		}while(r2>=1 || r2==0);

		x=x1*sqrt(-2*log(r2)/r2);				
		y=y1*sqrt(-2*log(r2)/r2);

		redo2=y;
		todo2=false;
		return x;
	}
	else{
		todo2=true;	
		return redo2;	
	}
}
