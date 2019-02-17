#include<iostream>
#include<fstream>
#include<cmath>
#include<iomanip>
#include "gauss_legendre.h"
using namespace std;

#define W 18

const double exact= exp(1)-1;
double trapezoid(int n, double x_in, double x_fin, func& f);
double simpson(int n, double x_in, double x_fin, func& f);

int main(){
	func f;
	double app_value;
	int n=0;
	
	ofstream out("gauss_simp_trapez.csv", ios::app);
	out.precision(15);
	if (out.tellp() == 0) out << "#order\t" << setw(W)<<"exact\t"<<setw(W)<<"gau_leg\t"<<setw(W)<<"err\t"<<setw(W)<<"trapez\t"<<setw(W)<<"err\t"<<setw(W)<<"simp\t"<<setw(W)<<"err\n";
	cout << "inserire ordine per gauss_legendre\n"; cin >> n;
	app_value=gauss_legendre(n, f, 0, 1);
	out << n << "\t" << setw(W) << exact << "\t" << setw(W)<< app_value << "\t" << setw(W) << abs(exact - app_value)/exact;
	
	app_value=trapezoid(n, 0, 1, f);	
	out << "\t" << setw(W) << app_value << "\t" << setw(W) << abs(exact - app_value)/exact;

	app_value=simpson(n, 0, 1, f);
	out << "\t" << setw(W) << app_value << "\t" << setw(W) << abs(exact - app_value)/exact<<endl;

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
