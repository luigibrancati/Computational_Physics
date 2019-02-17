#include<iostream>
#include<fstream>
#include<cmath>
#include<iomanip>
using namespace std;

class func{
public:
	double operator()(double x) {return exp(x);}
};

template<typename T>
T abs(T n){
	if (n>=0) return n;
	else return (-n);
}

double trapezoid(int, double, double, func&);
double simpson(int, double, double, func&);

int main(){
	int n=0;
	double trap=0, simp=0;	
	func f;		

	double x_min=0, x_max=1;
	//cout <<"estremo inf: "; cin>> x_min;
	//cout <<"estremo sup: "; cin>>x_max;
	
	const double int_value=f(x_max)-f(x_min);

	ofstream out("trap_simp_int.csv");
	out <<setw(10)<<"#interval"<<"\t"<< setw(10)<<"integral"<<"\t"<< setw(10)<<"trapez"<<"\t"<< setw(10)<<"trap_err"<<"\t"<< setw(10)<<"simpson"<<"\t"<< setw(10)<<"simps_err\n";
	for(int i=2; i<8; i++){
		n=pow(2,i);
		trap=trapezoid(n, x_min, x_max, f);
		simp=simpson(n, x_min, x_max, f);

		out << setw(10)<< n <<"\t"<< setw(10)<<int_value <<"\t"<< setw(10)<< trap <<"\t"<< setw(10)<< abs(int_value-trap)//
		<<"\t"<< setw(10)<< simp << "\t"<< setw(10)<< abs(int_value - simp) <<endl;
	}
	out.close();

	return 0;
}

double trapezoid(int n, double x_in, double x_fin, func& f){

	double h=(double) abs((x_fin-x_in)/n);
	double F_trap=0;
	for(int i=1; i<n; i++) F_trap+=f(x_in+(i*h));

	F_trap=h*(F_trap+(1./2.)*(f(x_in)+f(x_in+n*h)));
 
	return F_trap;
}

double simpson(int n, double x_in, double x_fin, func& f){

	double h=(double) abs(x_fin-x_in)/n;
	double F_simp=0;
	for(int i=1; i<n; i++) F_simp+=(2)*pow(2,i%2)*f(x_in+i*h);

	F_simp= (h/3.)*(F_simp+(f(x_in)+f(x_in+n*h)));

	return F_simp;
}
