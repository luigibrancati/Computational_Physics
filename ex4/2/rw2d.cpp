#include<iostream>
#include<fstream>
#include<cmath>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<iomanip>
using namespace std;

typedef vector<double> vec;

double module(vector<double> R){
	double module=0;
	for(int i=0; i<R.size(); i++) module+=R[i]*R[i];
	return sqrt(module);
}

void algorithm_1(int, int, vector< vec >&, vec&, vec&);
void algorithm_2(int, int, vector< vec >&, vec&, vec&);
void algorithm_3(int, int, vector< vec >&, vec&, vec&);
void algorithm_4(int, int, vector< vec >&, vec&, vec&);
void algorithm_5(int, int, vector< vec >&, vec&, vec&);

int main(){
	int steps, walkers; 
	cout << "steps: "; cin>>steps;
	cout <<"walkers: "; cin>>walkers;

	srand(time(NULL));

	vector< vec > RN(2, vec(steps,0));
	vec R2N(steps,0);
	vec dR2N(steps,0);

	algorithm_1(steps, walkers, RN, R2N, dR2N);
	algorithm_2(steps, walkers, RN, R2N, dR2N);
	algorithm_3(steps, walkers, RN, R2N, dR2N);	
	algorithm_4(steps, walkers, RN, R2N, dR2N);
	algorithm_5(steps, walkers, RN, R2N, dR2N);
	
	return 0;
}

//sceglie un valore random di angolo in [0:2pi] e lo usa come angolo per la direzione di spostamento con modulo 1
void algorithm_1(int steps, int walkers, vector< vec >& RN, vec& R2N, vec& dR2N){
	double theta;
	double x, y;

	
	ofstream out("alg1.csv");
	for(int w=0; w<walkers; w++){
		out << ((w==0) ? ("\n\"walker number: ") : ("\n\n\"walker number: ")) <<w<<"\""<<endl<<"#steps\tx\ty\n";
		theta=0; x=0; y=0;
		for(int s=0; s<steps; s++){
			
			theta=(double)2*M_PI*rand()/RAND_MAX;
			x+=cos(theta);
			y+=sin(theta);
			
			RN[0][s]+=x; RN[1][s]+=y;
			R2N[s]+=(x*x+y*y);			

			out<< s <<"\t"<<x <<"\t" << y <<endl;
		}
	}
	out.close();

	ofstream out1("R_alg1.csv");	
	for(int s=0; s<steps; s++){
		RN[0][s]=RN[0][s]/walkers; RN[1][s]=RN[1][s]/walkers;
		R2N[s]=R2N[s]/walkers;
		dR2N[s]=R2N[s]-(RN[0][s])-(RN[1][s]);
	
		out1 <<setw(10)<< RN[0][s] << "\t" <<setw(10)<<RN[1][s]<<"\t"<<setw(10)<<R2N[s]<<"\t"<<setw(10)<<dR2N[s]<<endl;	
	}
	out1.close();
}

//sceglie un valore random in [-1:1] e ricava l'altro spostamento in modo da avere modulo 1 dello spostamento
void algorithm_2(int steps, int walkers, vector< vec >& RN, vec& R2N, vec& dR2N){
	double x, y, dx, dy;
	int sign;

	ofstream out("alg2.csv");
	for(int w=0; w<walkers; w++){
		out << ((w==0) ? ("\n\"walker number: ") : ("\n\n\"walker number: ")) <<w<<"\""<<endl<<"#steps\tx\ty\n";
		x=0; y=0; dx=0; dy=0;
		for(int s=0; s<steps; s++){
			dx=2.*((double)rand()/RAND_MAX) - 1;
			sign= ( (((double)rand()/RAND_MAX)>=0.5) ? 1 : -1 );			
			dy=(double)sign*sqrt(1-dx*dx);
			
			x+=dx;
			y+=dy;

			RN[0][s]+=x; RN[1][s]+=y;
			R2N[s]+=(x*x+y*y);
			out<< s << "\t" << x <<"\t" << y <<endl;
		}
	}
	out.close();

	ofstream out1("R_alg2.csv");	
	for(int s=0; s<steps; s++){
		RN[0][s]=RN[0][s]/walkers; RN[1][s]=RN[1][s]/walkers;
		R2N[s]=R2N[s]/walkers;
		dR2N[s]=R2N[s]-(RN[0][s])-(RN[1][s]);
	
		out1 <<setw(10)<< RN[0][s] << "\t" <<setw(10)<<RN[1][s]<<"\t"<<setw(10)<<R2N[s]<<"\t"<<setw(10)<<dR2N[s]<<endl;	
	}
	out1.close();
}

//sceglie due valori random in [-1:1] e li normalizza per avere uno spostamento di modulo 1
void algorithm_3(int steps, int walkers, vector< vec >& RN, vec& R2N, vec& dR2N){
	double x, y, dx, dy, l;

	ofstream out("alg3.csv");
	for(int w=0; w<walkers; w++){
		out << ((w==0) ? ("\n\"walker number: ") : ("\n\n\"walker number: ")) <<w<<"\""<<endl<<"#steps\tx\ty\n";
		x=0; y=0; dx=0; dy=0; l=0;
		for(int s=0; s<steps; s++){
			do{			
				dx=2.*((double)rand()/RAND_MAX) - 1;
				dy=2.*((double)rand()/RAND_MAX) - 1;
			}while(dx==0 && dy==0);
			l=sqrt(dx*dx+dy*dy);
			
			dx=dx/l;
			dy=dy/l;		
			x+=dx;
			y+=dy;

			RN[0][s]+=x; RN[1][s]+=y;
			R2N[s]+=(x*x+y*y);
			out<< s << "\t" << x <<"\t" << y <<endl;
		}
	}
	out.close();

	ofstream out1("R_alg3.csv");	
	for(int s=0; s<steps; s++){
		RN[0][s]=RN[0][s]/walkers; RN[1][s]=RN[1][s]/walkers;
		R2N[s]=R2N[s]/walkers;
		dR2N[s]=R2N[s]-(RN[0][s])-(RN[1][s]);
	
		out1 <<setw(10)<< RN[0][s] << "\t" <<setw(10)<<RN[1][s]<<"\t"<<setw(10)<<R2N[s]<<"\t"<<setw(10)<<dR2N[s]<<endl;	
	}
	out1.close();
}

//NORD,SUD,EST,OVEST
void algorithm_4(int steps, int walkers, vector< vec >& RN, vec& R2N, vec& dR2N){
	double x, y;
	int dir;

	ofstream out("alg4.csv");
	for(int w=0; w<walkers; w++){
		out << ((w==0) ? ("\n\"walker number: ") : ("\n\n\"walker number: ")) <<w<<"\""<<endl<<"#steps\tx\ty\n";
		x=0; y=0;
		for(int s=0; s<steps; s++){
			dir=(int)4*((double)rand()/RAND_MAX);			
			
			switch(dir){
				case(0): x+=1; break;
				case(1): y+=1; break;
				case(2): x-=1; break;
				case(3): y-=1; break;
			}
			
			RN[0][s]+=x; RN[1][s]+=y;
			R2N[s]+=(x*x+y*y);
			out<< s << "\t" << x <<"\t" << y <<endl;
		}
	}
	out.close();

	ofstream out1("R_alg4.csv");	
	for(int s=0; s<steps; s++){
		RN[0][s]=RN[0][s]/walkers; RN[1][s]=RN[1][s]/walkers;
		R2N[s]=R2N[s]/walkers;
		dR2N[s]=R2N[s]-(RN[0][s])-(RN[1][s]);
	
		out1 <<setw(10)<< RN[0][s] << "\t" <<setw(10)<<RN[1][s]<<"\t"<<setw(10)<<R2N[s]<<"\t"<<setw(10)<<dR2N[s]<<endl;	
	}
	out1.close();
}

//prende due valori in [-sqrt(2):sqrt(2)], il passo non ha modulo 1 singolarmente ma in media sì
void algorithm_5(int steps, int walkers, vector< vec >& RN, vec& R2N, vec& dR2N){
	double x, y, dx, dy;

	ofstream out("alg5.csv");
	for(int w=0; w<walkers; w++){
		out << ((w==0) ? ("\n\"walker number: ") : ("\n\n\"walker number: ")) <<w<<"\""<<endl<<"#steps\tx\ty\n";
		x=0; y=0; dx=0; dy=0;
		for(int s=0; s<steps; s++){
			dx=2*sqrt(3/2)*((double)rand()/RAND_MAX) - sqrt(3/2);
			dy=2*sqrt(3/2)*((double)rand()/RAND_MAX) - sqrt(3/2);		
					
			x+=dx;
			y+=dy;
			
			RN[0][s]+=x; RN[1][s]+=y;
			R2N[s]+=(x*x+y*y);
			out<< s << "\t" << x <<"\t" << y <<endl;
		}
	}
	out.close();
	
	ofstream out1("R_alg5.csv");	
	for(int s=0; s<steps; s++){
		RN[0][s]=RN[0][s]/walkers; RN[1][s]=RN[1][s]/walkers;
		R2N[s]=R2N[s]/walkers;
		dR2N[s]=R2N[s]-(RN[0][s])-(RN[1][s]);
	
		out1 <<setw(10)<< RN[0][s] << "\t" <<setw(10)<<RN[1][s]<<"\t"<<setw(10)<<R2N[s]<<"\t"<<setw(10)<<dR2N[s]<<endl;	
	}
	out1.close();
}
