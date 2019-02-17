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

int main(){
	int steps_min, steps_max, steps_tic, walkers_min, walkers_max, walkers_tic; //walkers è il numero di test da fare, steps sono gli intervalli temporali
	double p_back=0.5;	
	cout<<"steps min: "; cin>>steps_min;
	cout<<"steps max: "; cin>>steps_max;
	cout<<"steps tic: "; cin>>steps_tic;	
	cout <<"walkers min: "; cin>>walkers_min;
	cout<<"walkers max: "; cin>>walkers_max;
	cout<<"walkers tic: "; cin>>walkers_tic;	
	srand(time(NULL));

	for(int s=steps_min; s<=steps_max; s+=steps_tic){
		for(int w=walkers_min; w<=walkers_max; w+=walkers_tic){

			vec x_N(s,0);
			vec x2_N(s,0);
			vec dx2(s,0);
			vec accuracy(s,0);
			vec P_N(2*s+1,0);
			
			random_walk_1d(s, w, p_back, x_N, x2_N, dx2, accuracy, P_N);		
			
			ofstream out("mean_val.txt");
			out <<"#walkers: "<<w<<"  steps: "<<s<<"  valori medi su ogni istante di tempo"<<endl;
			out<<"#I\tx_N\tx2_N\t(dx_N)^2\taccuratezza\n";	

			for(int i=0; i<x_N.size(); i++){
				out << i << "\t"<<x_N.at(i)<<"\t"<<x2_N.at(i)<<"\t"<<dx2.at(i)<<"\t"<<accuracy.at(i)<<endl;
				if(i==x_N.size()-1) out << endl<<endl;
			}			
			out.close();

			ofstream out1("prob.txt");
			out<<"\n\n#probabilità\n";		
			for(int i=0; i<P_N.size(); i++)	out1<<i-s<<"\t"<<double(P_N.at(i)/w)<<"\t"<<PN_theoretical(s, i-s, p_back)<<endl;		
			out1.close();		
		}
	}	
	return 0;
}

double PN_theoretical(int n, int p, double p_back){
	double PN=0, coeff=0;
	if((n+p)%2==0){
		coeff=(double)(fact(n)/(fact((n+p)/2.)*fact((n-p)/2.)));	
	
		PN=coeff*pow(p_back, (n-p)/2.)*pow((1-p_back),(n+p)/2.);	

		return (double)PN;
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


