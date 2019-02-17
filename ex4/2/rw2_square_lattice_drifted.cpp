#include<iostream>
#include<fstream>
#include<cmath>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<iomanip>
using namespace std;

typedef vector<double> vec;

void algorithm_4(int, int, float, float, float, float, vector< vec >&, vec&, vec&);
int choose_dir(float, float, float, float);

int main(){
	int steps, walkers; 
	float p_up=0, p_down=0, p_left=0, p_right=0;
	cout << "steps: "; cin>>steps;
	cout <<"walkers: "; cin>>walkers;
	
	do{
		cout<<"p up: "; cin >>p_up;
		cout<<"p down: "; cin >>p_down;
		cout<<"p left: "; cin >>p_left;	
		cout<<"p right: "; cin >>p_right;
		
		if((p_up+p_down+p_left+p_right)==1) break;
		else cout << "errore\n"; 		

	}while(true);
	srand(time(NULL));

	vector< vec > RN(2, vec(steps,0));
	vec R2N(steps,0);
	vec dR2N(steps,0);
	
	algorithm_4(steps, walkers, p_up, p_down, p_left, p_right, RN, R2N, dR2N);	

	return 0;
}

//NORD,SUD,EST,OVEST
void algorithm_4(int steps, int walkers, float p_up, float p_down, float p_left, float p_right, vector< vec >& RN, vec& R2N, vec& dR2N){
	double x, y;
	
	ofstream out("alg_sl_drift.csv");
	for(int w=0; w<walkers; w++){
		out<<"\n\n#walker: "<<w<<endl;
		x=0; y=0;
		for(int s=0; s<steps; s++){
			
			switch(choose_dir(p_up, p_down, p_left, p_right)){
				case(0): x+=1; break; //right
				case(1): y-=1; break; //down
				case(2): x-=1; break; //left
				case(3): y+=1; break; //up
			}
			
			RN[0][s]+=x; RN[1][s]+=y;
			R2N[s]+=(x*x+y*y);
			out<< s << "\t" << x <<"\t" << y <<endl;
		}
	}
	out.close();

	ofstream out1("R_alg_sl_drift.csv");	
	for(int s=0; s<steps; s++){
		RN[0][s]=RN[0][s]/walkers; RN[1][s]=RN[1][s]/walkers;
		R2N[s]=R2N[s]/walkers;
		dR2N[s]=R2N[s]-(RN[0][s])-(RN[1][s]);
	
		out1 <<s<<"\t"<<setw(10)<< RN[0][s] << "\t" <<setw(10)<<RN[1][s]<<"\t"<<setw(10)<<R2N[s]<<"\t"<<setw(10)<<dR2N[s]<<endl;	
	}
	out1.close();
}

int choose_dir(float p_up, float p_down, float p_left, float p_right){
	float ran=(float) rand()/RAND_MAX;
	
	if(ran < p_right) return 0;
	else if (ran>p_right && ran<(p_right+p_down)) return 1;
	else if (ran>(p_right+p_down) && ran<(p_right+p_down+p_left)) return 2;
	else return 3;
}

