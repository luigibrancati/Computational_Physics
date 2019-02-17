#ifndef RW1D_H_
#define RW1D_H_

#include<fstream>
#include<vector>

template<typename T>
T abs(T n){
	if(n>=0) return n;
	else return -n;
}

void random_walk_1d(int steps, int walkers, float p_back, std::vector<float>& x_N, std::vector<float>& x2_N, std::vector<float>& dx2, std::vector<float>& acc, std::vector<float>& P_N){
	
	int xi=0;

	std::ofstream out("rw1d.txt");
	for(int w=0; w<walkers; w++){
		xi=0;
		out << ((w==0) ? ("\n\"walker number: ") : ("\n\n\"walker number: ")) <<w<<"\""<<std::endl;
		out << "#step\txi\txi^2\n";
		for(int i=0; i<steps; i++){
			if(((float)rand()/RAND_MAX) < p_back) xi+=1;	
			else xi-=1;

			x_N.at(i)+=xi;		
			x2_N.at(i)+=xi*xi;
			out<< i << "\t"<<xi<<"\t"<<xi*xi<<std::endl;
		}
		P_N.at(xi+steps)+=1;		
	}

	for (int i=0; i<steps; i++){
		x_N.at(i)=float(x_N.at(i)/walkers);
		x2_N.at(i)=float(x2_N.at(i)/walkers);
		dx2.at(i)=x2_N.at(i)-(x_N.at(i)*x_N.at(i));
		acc.at(i)=abs(float(dx2.at(i)/(i+1))-1);
	}
	out.close();
}


#endif
