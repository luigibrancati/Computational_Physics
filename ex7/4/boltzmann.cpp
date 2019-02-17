#include<iomanip>
#include<cmath>
#include<cstdlib>
#include<vector>
#include<ctime>
#include "/home/luigi/Fisica_computazionale/histogram.h"
using namespace std;

#define W 10

const double dv_val[12]={9,12,16,18.5,21,22.5,24.5,26.5,27.5,29.5,31,32};

class func{
	double T;
public: 
	func(double x): T(x) {}//passo la temperatura
	double operator()(double x){return exp(-(x*x)/(2*T));}
};

double metropolis(double, double, int&, func&);
double determine_dv(int, func&, double);

int main(){
	int acc=0, nmcs=20000, N=20; //numero di passi (tempo) e di particelle considerato
	double dv, C=0, v0=0, T=10, mean_E=0, mean_v=0, var_E=0, var_v=0;
	bool C_full=false;

	double ** v=new double*[N];
	for(int i=0; i<N; i++) v[i]=new double[nmcs];
	
	double * E=new double[nmcs];

	srand(time(NULL));


	for(int k=0; k<=11; k++){
	T=10*(k+1);
	func f(T);
	dv=dv_val[k];

	for(int i=0; i<nmcs; i++){
		for(int j=0; j<N; j++) v[j][i]=0;
		E[i]=0;
	}	

	for(int i=0; i<N; i++){
		v[i][0]=v0;
		E[0]+=0.5*v[i][0]*v[i][0];
	}

	mean_v=0; var_v=0; mean_E=0; var_E=0;	

	ofstream out("distrib.csv");
	ofstream out2("E_v_on_T.csv", ios::app);
	
	for(int i=1; i<nmcs; i++){

		for(int j=0; j<N; j++){
			v[j][i]=metropolis(v[j][i-1], dv, acc, f);
			E[i]+=0.5*v[j][i]*v[j][i];
			mean_v+=v[j][i]/N;
			var_v+=v[j][i]*v[j][i]/N;
		}
	
		mean_E+=E[i];
		var_E+=E[i]*E[i];

		out << i << "\t" << setw(W) << E[i] << endl;
	}

	//histogram(v, nmcs, out);
	//histogram(E, nmcs, out);

	mean_v=mean_v/nmcs;
	var_v=var_v/nmcs - mean_v*mean_v;
	mean_E=mean_E/nmcs;
	var_E= var_E/nmcs - mean_E*mean_E;
	
	out2 << T << "\t"<<setw(W)<< mean_v << "\t"<< setw(W)<< sqrt(var_v/nmcs) << "\t" << setw(W) << mean_E << "\t" << setw(W) << var_E/(T*T) << endl;	

	ofstream out3("specific_heat.csv", ios::app);
	static double C=-mean_E;
	C+=mean_E;
	C=C/10;

	out3 << T-10 << "\t" << setw(W) << C << endl;

	C=-mean_E;	
	out3.close();
		
	out2.close();
	out.close();
	}
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

//determina il valore di dv più vicino a 50% di accettazione (exp_accept)
double determine_dv(int T, func& f, double exp_accept){
	double dv=0, dv_err=0, dv_err_min=1, x=0; 
	int accept=0, nmcs=100000;
	
	ofstream out("dv_det_test.csv");

	vector<float> dv_val;
	for(double i=0; i<=100; i+=0.5)	dv_val.push_back(i);	
	
	cout << "algoritmh\n";
	for(int j=0; j<dv_val.size(); j++){
		
		x=0; accept=0;
		for(int i=0; i<nmcs; i++) x=metropolis(x, dv_val.at(j), accept, f);


		dv_err=abs((double)accept/nmcs-exp_accept);	

		out << dv_val[j] << "\t" << (double)accept/nmcs << "\t" << dv_err << endl;
	
		if(dv_err<dv_err_min) {
			dv_err_min=dv_err;
			dv=dv_val.at(j);
		}	
	}
	
	return dv;
}
