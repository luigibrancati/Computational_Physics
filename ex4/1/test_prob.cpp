#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<vector>
using namespace std;

typedef std::vector<double> vec;

template<typename T>
T abs(T n){
	if(n>=0) return n;
	else return -n;
}

int fact(int n){
	if(n==0) return 1;
	else n=n*fact(n-1);
	return n;
}

double PN_theoretical(int, int, double);
void random_walk_1d(int, int, double, vec&, vec&, vec&, vec&, vec&);
double PN_gauss(vec&, vec&, double);

int main(){
	int steps, walkers; //walkers è il numero di test da fare, steps sono gli intervalli temporali
	double p_back=0.5;	
	cout<<"steps: "; cin>>steps;
	cout <<"walkers: "; cin>>walkers;

	srand(time(NULL));

			vec x_N(steps,0);
			vec x2_N(steps,0);
			vec dx2(steps,0);
			vec accuracy(steps,0);
			vec P_N(2*steps+1,0);
			
			random_walk_1d(steps, walkers, p_back, x_N, x2_N, dx2, accuracy, P_N);		
			
			ofstream out("mean_val.txt");
			out <<"#walkers: "<<walkers<<"  steps: "<<steps<<"  valori medi su ogni istante di tempo"<<endl;
			out<<"#I\tx_N\tx2_N\t(dx_N)^2\taccuratezza\n";	

			for(int i=0; i<x_N.size(); i++){
				out << i << "\t"<<x_N.at(i)<<"\t"<<x2_N.at(i)<<"\t"<<dx2.at(i)<<"\t"<<accuracy.at(i)<<endl;
				if(i==x_N.size()-1) out << endl<<endl;
			}			
			out.close();

			ofstream out1("prob.txt");
			out<<"\n\n#probabilità\n";		
			for(int i=0; i<P_N.size(); i++)	out1<<i-steps<<"\t"<<double(P_N.at(i)/walkers)<<"\t"<<PN_theoretical(steps, i-steps, p_back)<<"\t"<<PN_gauss(x2_N, x_N, i-steps)<<endl;		
			out1.close();		
	return 0;
}

double PN_gauss(vec& x2_N, vec& x_N, double pos){
	double sigma2=x2_N.back(); 
	double mean=x_N.back();	
	double coeff=1/sqrt(2*M_PI*sigma2);
	double P_N=coeff*exp(-pow((pos-mean),2)/(2*sigma2));

	return P_N;
}

double PN_theoretical(int n, int p, double p_back){
	double PN=0, num=0, den=0;
	if((n+p)%2==0){
		num=fact(n);
		den=fact((n+p)/2.)*fact((n-p)/2.);	
		cout << num << "  " << den << endl;

		PN=(num/den)*pow(p_back, (n-p)/2.)*pow((1-p_back),(n+p)/2.);	

		return PN;
	}
	else return 0;
}

void random_walk_1d(int steps, int walkers, double p_back, vec& x_N, vec& x2_N, vec& dx2, vec& acc, vec& P_N){
	
	int xi=0;

	std::ofstream out("rw1d.txt");
	for(int w=0; w<walkers; w++){
		xi=0;
		out << ((w==0) ? ("\n\"walker number: ") : ("\n\n\"walker number: ")) <<w<<"\""<<std::endl;
		out << "#step\txi\txi^2\n";
		for(int i=0; i<steps; i++){
			if(((double)rand()/RAND_MAX) < p_back) xi+=1;	
			else xi-=1;

			x_N.at(i)+=xi;		
			x2_N.at(i)+=xi*xi;
			out<< i << "\t"<<xi<<"\t"<<xi*xi<<std::endl;
		}
		P_N.at(xi+steps)+=1;		
	}

	for (int i=0; i<steps; i++){
		x_N.at(i)=double(x_N.at(i)/walkers);
		x2_N.at(i)=double(x2_N.at(i)/walkers);
		dx2.at(i)=x2_N.at(i)-(x_N.at(i)*x_N.at(i));
		acc.at(i)=abs(double(dx2.at(i)/(i+1))-1);
	}
	out.close();
}
