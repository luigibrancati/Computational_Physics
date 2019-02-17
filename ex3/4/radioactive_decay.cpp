#include<iostream>
#include<fstream>
#include<cmath>
#include<cstdlib>
#include<ctime>
using namespace std;

void sim_decay(int, double);

int main(){
	int nstart=0;
	double lambda=0;
	cout<<"lambda: "; cin>>lambda;
	cout<<"N start: "; cin>>nstart;
		
	srand(time(NULL));	
	sim_decay(nstart, lambda);
	
	return 0;
}

void sim_decay(int nstart, double lambda){
	int time=1;	
	int nleft=nstart;
	int nleft_temp=0;
	double x;
	ofstream out("decay.txt");
	out<< 0 <<"\t"<< (double) nleft/nstart <<endl;
	do{
		nleft_temp=nleft;
		for(int i=0; i<nleft_temp; i++){
			x=(double) rand()/RAND_MAX;
			if(x <= lambda) nleft-=1;
		}
		out<<time<<"\t"<< (double) nleft/nstart <<endl;
		time+=1;
	}while(nleft>0);
	out.close();
}
