#include<iostream>
#include<fstream>
#include<cmath>
#include<iomanip>
#include<cstdlib>
#include<ctime>
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
double gaussian1();
double mean(double*, int);
double variance(double*, int);

int main(){
	int n=0;
	cout <<"lunghezza serie: "; cin>>n;

	ofstream out("gaussian1.txt");
	double * x=new double[n];
	srand(time(NULL));
	for(int i=0; i<n; i++){
		x[i]=gaussian1();
		out<<x[i]<<endl;
	}	

	out<<"\n\n#mean= " << mean(x,n) <<endl;
	out<<"#variance= "<< variance(x,n) <<endl;	

	out.close();

	histogram(x, n);	

	return 0;
}

double mean(double * rng, int dim){
	double sum=0;

	for(int i=0; i<dim; i++)sum+=rng[i];

	return (double)sum/dim;
}

double variance(double * rng, int dim){
	double variance_v=0;
	double mean_v=mean(rng, dim);
	double sum=0;	
	
	for(int i=0; i<dim; i++) sum+=rng[i]*rng[i];
	sum=(double) sum/dim;
	variance_v=sum-mean_v*mean_v;
	
	return variance_v;
}

void histogram(double * rng, int dim){
	int nh=0;
	cout<<"inserire numero intervalli: "; cin >> nh;	
	
	double xmax=max(rng, dim);
	double xmin=min(rng, dim);
	double delta=(float)(xmax-xmin)/nh;	
	int checksum=0;
	
	if(xmin<0){
		for(int i=0; i<dim; i++) rng[i]-=xmin;
	}

	double * h=new double[nh];
	for(int i=0; i<nh; i++) h[i]=0;

	for(int i=0; i<dim; i++) h[int(rng[i]/delta)]+=1;
	
	ofstream out("histogrambasic.txt");
	out << "\n\n#histogram\n";
	for(int i=0; i<nh; i++){
		out<< setw(W) << (double) i*delta+xmin << "\t" << setw(W) << (double) h[i]/(dim*delta) <<endl;
		checksum+=h[i];
	}
	out<<"\n#checksum= "<<checksum<<endl;
	out.close();
}

double gaussian1(){
	static bool todo1=true;
	static double redo1;
	double x1, y1, x, y;	

	if(todo1){
		x1=(double)rand()/RAND_MAX;
		y1=(double)rand()/RAND_MAX;
			
		x=sqrt(-2*log(x1))*cos(2*M_PI*y1);
		y=sqrt(-2*log(x1))*sin(2*M_PI*y1);
		

		redo1=y;
		todo1=false;
		return x;		
	}
	else{
		todo1=true;
		return redo1; 
	}
}

