#include<iostream>
#include<fstream>
#include<cmath>
#include<iomanip>
#include<cstdlib>
using namespace std;

class func{
public:
	double operator()(double x) {return sqrt(1-x*x);}
};

template<typename T>
T abs(T n){
	if(n>=0) return n;
	else return (-n);
}

void sample_mean(int, double&, double&);
double sm(int);
void avg_of_avg(int, double&, double&);
void block_avg(int, double&, double&);

int main(){
	func f;	
	double F_N=0, sigma=0;
	int n, sw;

	cout<<"switch (1,2,3): "; cin>>sw;

	ofstream out("err_an.csv");
	out<<setw(10)<<"#n\t"<<setw(10)<<"F_N\t"<<setw(10)<<"err\t"<<setw(10)<<"sigma"<<setw(10)<<"sigma_n"<<endl;	
		
		switch(sw){
		case(1):
			out<<"#sample_mean\n";
			for(int i=2; i<=6; i++){
				n=pow(10, i);
				sample_mean(n, F_N, sigma);		
				out<< setw(10)<<n<<"\t"<<setw(10)<<4*F_N<<"\t"<<setw(10)<<abs(M_PI-4*F_N)<<"\t"<<setw(10)<<sigma<<"\t"<<setw(10)<<sigma/sqrt(n)<<endl;
			}
			break;
		case(2):
			out << "#avg_of_avg\n";
			for(int i=4; i<=6; i++){
				n=pow(10, i);			
				avg_of_avg(n, F_N, sigma);
				out<< setw(10)<<n<<"\t"<<setw(10)<<4*F_N<<"\t"<<setw(10)<<abs(M_PI-4*F_N)<<"\t"<<setw(10)<<sigma<<"\t"<<endl;
			}			
			break;
		case(3):
			out<<"#block_avg\n";
			for(int i=2; i<=6; i++){
				n=pow(10, i);
				block_avg(n, F_N, sigma);
				out<< setw(10)<<n<<"\t"<<setw(10)<<4*F_N<<"\t"<<setw(10)<<abs(M_PI-4*F_N)<<"\t"<<setw(10)<<sigma<<"\t"<<endl;		
			}
			break;
		}		
	
	out.close();

	return 0;
}

void sample_mean(int n, double& F_N, double& sigma){
	double x;
	func f;	

	for(int i=0; i<n; i++){
		x=(double) rand()/RAND_MAX;
		F_N+=f(x);
		sigma+=f(x)*f(x);
	}

	F_N=(double) F_N/n;
	sigma=(double)(sigma/n - F_N*F_N);
}


double sm(int n){
	double x=0, mean=0;
	func f;

	for(int i=0; i<n; i++){
		x=(double) rand()/RAND_MAX;
		mean+=f(x);
	}

	return (double) mean/n;
}

void avg_of_avg(int n, double& F_N, double& sigma){
	double temp=0;
	int m=10;	//trials
	func f;	

	F_N=0; sigma=0;
	for(int i=0; i<m; i++){
		temp=sm(n);
		F_N+=temp;
		sigma+=temp*temp;
	}
	
	F_N= F_N/m;
	cout << 4*F_N<<endl;
	sigma= sigma/m - F_N*F_N;
}

void block_avg(int n, double& F_N, double& sigma){
	double temp=0;	
	int m=10; //subsets	
	func f;
	
	F_N=0; sigma=0;
	for(int i=0; i<m; i++){
		temp=sm(n/m);
		F_N+=temp;
		sigma+=temp*temp;
	}
	
	F_N= F_N/m;
	cout << 4*F_N<<endl;
	sigma= sigma/m - F_N*F_N;
}

