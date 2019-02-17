/* Algoritmo per calcolare l'autocorrelazione di due distribuzioni di gauss generate una con l'algoritmo di Box-Muller e l'altra con Metropolis.
*/

#include<iostream>
#include<fstream>
#include<cmath>
#include<iomanip>
#include<cstdlib>
#include<ctime>
#include<vector>
using namespace std;

#define W 10
#define SIG 1
#define jmax 30

//classe per generare funzione gaussiana con parametro libero
class func{
	double sigma;
public:
	func(double x): sigma(x) {cout << "generato\n";}
	double operator()(double x){return exp(-(x*x)/(2*sigma*sigma));}
};

double gaussian2(); //Box Muller per dist gaussian
double metropolis(double, double, func&); //metropolis per distribuzione gaussiana
void block_average(double*, int, double, ofstream&);

int main(){
	func f(SIG);//Gaussiana con sigma=SIG definito sopra
	int n=10000;//n steps
	double x0=0, var=0, delta=5*SIG, C=0, x1=0, x2=0; //x0 e delta sono variabili utili per l'alg. metropolis, C=autocorrelazione e x1,x2 servono per calcolare C. Var è la varianza

	double * x_bm=new double[n];
	double * x_me=new double[n];

	x_me[0]=x0; x_bm[0]=x0;
	for(int i=1; i<n; i++){
		x_bm[i]=gaussian2()*SIG;
		x_me[i]=metropolis(x_me[i-1], delta, f);
	}
	
	ofstream out1("box_muller.csv");
	out1<<"#box_muller\n";
	x1=0; x2=0; C=0;

	for(int j=0; j<jmax; j++){
		for(int i=0; i<n-j; i++) C+=x_bm[i]*x_bm[i+j];
		for(int i=0; i<n; i++){
			x1+=x_bm[i];
			x2+=x_bm[i]*x_bm[i];
		}
		x1=x1/n;
		x2=x2/n;
		C=(C/(n-j) - x1*x1)/(x2-x1*x1);
		out1 << j << "\t" << setw(W) << C << endl; 
	}		

	var=x2 - x1*x1;
	var=sqrt(var/n);
	block_average(x_bm, n, var, out1);
	out1.close();

	ofstream out2("metrop.csv");
	out2 << "#metropolis\n";
	x1=0; x2=0; C=0;

	for(int j=0; j<jmax; j++){
		for(int i=0; i<n-j; i++) C+=x_me[i]*x_me[i+j];		
		for(int i=0; i<n; i++){
			x1+=x_me[i];
			x2+=x_me[i]*x_me[i];
		}
		x1=x1/n;
		x2=x2/n;
		C=(C/(n-j) - x1*x1)/(x2-x1*x1);
		out2 << j << "\t" << setw(W) << C << endl; 
	}
	
	var=x2 - x1*x1;
	var=sqrt(var/n);
	block_average(x_me, n, var, out2);
	out2.close();

	return 0;
}

//algoritmo di box muller per dist. gaussiana
double gaussian2(){
	//Alg. di Box-Muller, definisco le variabili static per dargli scope globale
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

//metropolis per dist. gaussiana
double metropolis(double x0, double delta, func& f){
	double passo=2*delta*((double) rand()/RAND_MAX) - delta;	
	double x_trial=x0 + passo;
	
	
	double w=(double) f(x_trial)/f(x0);


	if(w>=1) return x_trial;
	else{
		double r=(double) rand()/RAND_MAX;
		if(r<=w){
			return x_trial;
		}
		else return x0;
	}	
}

//block average per analisi statistica
void block_average(double * x, int n, double var1, ofstream& out){
	int n_bl=0;
	double var=0, mean=0;
	
	/*definisco i valori da considerare per il numero di blocchi da prendere, questo perchè è necessario che il numero di blocchi sia un divisore del numero totale di punti*/
	double s[5]={1, 10, 100, 1000, 10000}; //5 valori del numero di blocchi

	out << "\n\n#block average\n";
	for(int s_bl=0; s_bl<5; s_bl++){
		var=0; mean=0;
		n_bl=n/s[s_bl];
	
		vector<double> bl_mean(s[s_bl], 0);						

		for(int i=0; i<s[s_bl]; i++){
			for(int j=0; j<n_bl; j++){
				bl_mean[i]+=x[j+i*n_bl];	
			}
			bl_mean[i]=bl_mean[i]/n_bl;

			mean+=bl_mean[i];
			var+=bl_mean[i]*bl_mean[i];
		}		
		mean=mean/s[s_bl];
		var= var/s[s_bl] - mean*mean;
		
		out << s[s_bl] << "\t" << setw(W) << sqrt(var/s[s_bl]) << "\t" << setw(W) << var1 << endl;								
	}
}


