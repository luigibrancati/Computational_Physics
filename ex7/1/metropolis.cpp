#include<cmath>
#include<iomanip>
#include<cstdlib>
#include<ctime>
#include "/home/luigi/Fisica_computazionale/histogram.h"
using namespace std;

#define SIG 1
#define W 10

class func{
	double sigma;
public:
	func(double x): sigma(x) {cout << "generato\n";}
	double operator()(double x){return exp(-(x*x)/(2*sigma*sigma));}
};

double metropolis(double, double, int&, func&);

int main(){
	func p(SIG);
	int n=10000, acc=0;
	double x0, delta, x1, x2, x3, x4;
	cout << "punto di partenza: "; cin >> x0; 
	cout << "lunghezza passo: "; cin>> delta;
	//cout << "numero passi: "; cin>> n;
	delta=delta*SIG;

	srand(time(NULL));

	ofstream out("metropolis.csv");
	ofstream out2("momenta.csv");
	out2<<"#n\t"<<setw(W)<<"0-x1\t"<<setw(W)<<"sigma-x2\t"<<setw(W)<<"sig-x2 rel\t"<<setw(W)<<"x3\t"<<setw(W)<<"x4\t"<<endl;
		
	while(n<=100000){

		double * x=new double[n];
		acc=0;	
		x[0]=x0;
		x1=x0; x2=x0*x0; x3=x0*x0*x0; x4=x0*x0*x0*x0;
		
		out << "#passo\t" << setw(W) << "posiz"<< endl;
		out << 0 <<"\t"<< setw(W) << x[0] << endl;
	
		for (int i=1; i<n; i++){
			x[i]=metropolis(x[i-1], delta, acc, p);
			out << i << "\t"<< setw(W) << x[i] << endl;
			x1+=x[i];
			x2+=x[i]*x[i];
			x3+=x[i]*x[i]*x[i];
			x4+=x[i]*x[i]*x[i]*x[i];				 
		}
		
		//histogram(x, n, out);
/*
		ofstream out1("acceptance.csv", ios::app);
		out1 << delta/SIG << "\t" << ((double)acc/n) << endl;	
		out1.close();
*/
		out2 << n << "\t" << setw(W)<< 0-x1/n << "\t" <<setw(W)<< SIG-x2/n<<"\t"<<setw(W)<< abs(SIG-x2/n)/SIG << "\t" <<setw(W)<< x3/n << "\t" <<setw(W)<< x4/n << endl;
		
		n+=1000;
		cout << n << endl;
		delete[] x;
	};

	out2.close();
	out.close();
	return 0;
}

double metropolis(double x0, double delta, int& acc, func& f){
	double passo=2*delta*((double) rand()/RAND_MAX) - delta;	
	double x_trial=x0 + passo;
	
	double w=(double) f(x_trial)/f(x0);
	
	if(w>=1){
		acc+=1;
		return x_trial;
	}
	else{
		double r=(double) rand()/RAND_MAX;
		if(r<=w){
			acc+=1;
			return x_trial;
		}
		else return x0;
	}	
}
