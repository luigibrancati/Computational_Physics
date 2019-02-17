#include<iostream>
#include<fstream>
#include<cmath>
#include<iomanip>
#include<cstdlib>
using namespace std;

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

double unif_dist();
void histogram(double*,int,ofstream&);

int main(){
	int n_mean, n_points;
	cout << "numero punti per media: "; cin>>n_mean;
	cout<<"numero punti distrib:"; cin>>n_points;

	double * x=new double[n_points];
	double x_N_m=0, sigma_xn=0;
	ofstream out("gauss_dist.csv");	
	out << "#i\tpoint\n";
	for(int i=0; i<n_points; i++){
		x[i]=0;
		for(int j=0; j<n_mean; j++) x[i]+=unif_dist();
		x_N_m+=x[i];
		sigma_xn+=x[i]*x[i];
		out<<i<<"\t"<<setw(10)<<x[i]<<endl;
	}
	
	x_N_m= (double)x_N_m/n_points;
	sigma_xn= ((double)sigma_xn/n_points - x_N_m*x_N_m);
	
	out << "\n\n#mean\tsigma\n";
	out<<setw(10)<<x_N_m<<"\t"<<setw(10)<<sigma_xn<<endl;

	histogram(x, n_points, out);
	out.close();

	double * z=new double[n_points];
	double z_4=0, z_2=0;
	for (int i=0; i<n_points; i++){
		z[i]=(x[i]-n_mean)/n_mean;
		z_2+=z[i]*z[i];
		z_4+=z[i]*z[i]*z[i]*z[i];	
	}
	z_2=z_2/n_points;
	z_4=z_4/n_points;

	cout << z_4 << "\t" << (z_2*z_2)<<endl;

	return 0;
}

//genera le variabili tra [-1:1]
double unif_dist(){
	double x=0;
	double r=(double)rand()/RAND_MAX;
	x=M_PI * tan(r);
	return x;
}

void histogram(double * rng, int dim, ofstream& out){
	int nh=0;
	cout<<"inserire numero intervalli: "; cin >> nh;	
	
	float xmax=max(rng, dim);
	float xmin=min(rng, dim);
	float delta=(float)(xmax-xmin)/nh;	
	int checksum=0;
	
	if(xmin<0){
		for(int i=0; i<dim; i++) rng[i]-=xmin;
	}

	float * h=new float[nh];
	for(int i=0; i<nh; i++) h[i]=0;

	for(int i=0; i<dim; i++) h[(int)(rng[i]/delta)]+=1;
	
	out << "\n\n#histogram\n";
	for(int i=0; i<nh; i++){
		out<<i<<"\t"<<h[i]<<endl;
		checksum+=h[i];
	}
	out<<"\n#checksum= "<<checksum<<endl;
}
