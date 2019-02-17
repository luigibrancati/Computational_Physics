#include<iostream>
#include<fstream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<iomanip>
#include<vector>
using namespace std;

#define W 10

const double p_max=1.1547;

int N, nmcs=10000, g_count, accept;
double Lx, Ly, p, dxmax=0.2, dymax=0.2, delta=0.1, T, E, E_mean=0, E_var=0, cutoff;

int nbins;
double r_max;
vector<double> g; 

class potential{
public:	
	double operator()(double r){
		double pot;
		if (r<=cutoff) pot=4*(pow((double)1/r, 12)-pow((double)1/r,6));			
		else pot=0;		
		return pot;
	}
}u;

class particle{
public:
	particle(double a, double b): x(a), y(b) {}
	double x;
	double y;
};

vector<particle> sistem;

void print_particles_positions(vector<particle> par, ostream& out, int time){
	out << "\n\n#step: " << time << endl;
	for(int i=0; i<par.size(); i++) out << i << "\t" << setw(W) << par[i].x << "\t" << setw(W) << par[i].y << endl;
}

//posiziona le particelle in un lattice triangolare con densità massima e poi riscala le dimensioni e posizioni alla densità desiderata
void triangular_lattice(){
	do{	
		cout << "p (max: 1.1547): "; cin >> p;
		if (p>p_max) cout << "attenzione! p > p_max=1.1547! scegliere p minore\n";
	}while(p>p_max);	
	N=p*Lx*Ly;
	cout << "N: " << N << endl;
	sistem.resize(N, particle(0,0));
	double ratio1= (double) p/p_max;
	cout << "ratio: " << ratio1 << endl;

	//genera un lattice triangolare massimamente denso
	double Lx_t=(double) Lx*sqrt(ratio1);
	double Ly_t=(double) Ly*sqrt(ratio1);
	double dx=0, dy=0;
	int m=0;
	bool shift=false;
	while(dy<=Ly_t){							
		if (shift){ 
			dx=0.5;
			shift=false;
		}
		else{
			dx=0;
			shift=true;
		}
		while(dx<=Lx_t){
			if(m>=N) break;
			sistem[m].x=0.5+dx;
			sistem[m].y=0.5+dy;
			dx+=1;
			m++;
		}
		if(m>=N) break;	
		dy+=0.5*sqrt(3);
	}

	//riscala
	for(int i=0; i<N; i++){
		sistem[i].x=sistem[i].x*sqrt(1/ratio1);
		sistem[i].y=sistem[i].y*sqrt(1/ratio1);
	}
}
//riscala le posizioni usando le pbc
void PBC(double& xtrial, double& ytrial){
	xtrial-=floor(xtrial/Lx)*Lx;
	ytrial-=floor(ytrial/Ly)*Ly;
}

//riscala le distanze tra due particelle usando la image convention
void dist(double& dx, double& dy){
	dx-=Lx*((int) 2*dx/Lx);
	dy-=Ly*((int) 2*dy/Ly);
}

//controlla se ci sono overlay e in caso accetta la nuova posizione se non ne trova
void overlap_metro(int r, double xtrial, double ytrial, double xold, double yold){
	double w, random, dx1, dy1, dr1, dr2, dx2, dy2, dE=0;	
	for(int i=0; i<N; i++){
		if(i!=r){
			dx2=sistem[i].x-xtrial;
			dy2=sistem[i].y-ytrial;
			dist(dx2, dy2);
			dr2=sqrt(dx2*dx2+dy2*dy2);
			if(dr2<1){ 
				//cout << "non accettata\n";
				return;
			}
			
			dx1=xold-sistem[i].x;
			dy1=yold-sistem[i].y;
			dist(dx1, dy1);
			dr1=sqrt(dx1*dx1+dy1*dy1);
			dE+=(u(dr2)-u(dr1));
		}
	}
	//esegue lo step metropolism e se va bene aggiorna energia e posizione
	random=(double) rand()/RAND_MAX;
	w=((T==0)? 0:exp(-dE/T));
	if(random < w){
		E+=dE;
		sistem[r].x=xtrial;
		sistem[r].y=ytrial;
		accept+=1;
	}
	else return;
	
}

void move(){
	int r;
	double xtrial, ytrial, xold, yold, rand1, rand2;
	for(int i=0; i<N; i++){
		r=rand()%N;

		xold=sistem[r].x;
		yold=sistem[r].y;

		rand1=(double) rand()/RAND_MAX;
		rand2=(double) rand()/RAND_MAX;
		xtrial=xold + (2*rand1-1)*dxmax;
		ytrial=yold + (2*rand2-1)*dymax;
		PBC(xtrial, ytrial);
		overlap_metro(r, xtrial, ytrial, xold, yold);
	}
}

void thermalize(){
	int eqstep=0.2*nmcs;
	for(int i=1; i<eqstep; i++) move();
	accept=0;
}

//accumula g(r) ad ogni passo temporale
void accumulate_gr(){	
	int bin;
	double dx, dy, dr;		

	for(int i=0; i<N-1; i++){
		for(int j=i+1; j<N; j++){
			dx=sistem[i].x-sistem[j].x;	
			dy=sistem[i].y-sistem[j].y;
			dist(dx, dy);
			dr=dx*dx+dy*dy;
			dr=sqrt(dr);
			bin=dr/delta;			
			if (bin<nbins){
			 	g.at(bin)+=1;
			}
		}
	}
	g_count+=1;
	
}
//calcolo g(r)
void calculate_gr(){
	double normalization=2/(N*g_count*p);
	double shell_area, r;	
	ofstream out("gr.txt");
	for(int i=0; i<nbins; i++){
		r=i*delta;
		shell_area=M_PI*((r+delta)*(r+delta) - r*r);
		g[i]=g[i]*normalization/shell_area;
		out << i << "\t" << setw(W) << g[i] << endl;	
	}
	out.close();
}

void starting(){
	time_t seed=time(NULL);
	srand(seed);	
	cout << seed << endl;

	accept=0;
	//alloco le particelle in un lattice triangolare e inizializzo tutti i valori
	char choose;
	cout << "Lx: "; cin >> Lx;
	cout << "Ly: "; cin >> Ly;	
	cout << "T: "; cin >> T;
	triangular_lattice();
	
	r_max=min((double) Lx/2, (double) Ly/2);
	nbins=(r_max/delta);
	g.resize(nbins, 0);
	g_count=0;

	E=0;
	cutoff=r_max;
	double dx, dy, dr;
	for(int i=0; i<N-1; i++){
		for(int j=i+1; j<N; j++){
			dx=sistem[i].x-sistem[j].x;
			dy=sistem[i].y-sistem[j].y;
			dist(dx, dy);
			dr=dx*dx+dy*dy;
			dr=sqrt(dr);
			E+=u(dr);					
		}
	}
}

void MCLJ(ofstream& En, ofstream& part_pos){
	for(int i=1; i<nmcs; i++){
		move();
		accumulate_gr();
		if(i%50==0 && i<nmcs-1)	print_particles_positions(sistem, part_pos, i);	
		E_mean+=E;
		E_var+=E*E;		
		En << i << "\t" << setw(W) << E/N << "\t" << setw(W) << (double) E_mean/(N*i) << "\t" << setw(W) << (double) (E_var/i - (E_mean/i)*(E_mean/i))/(N*i) << endl;
	}	
	print_particles_positions(sistem, part_pos, nmcs);	
	calculate_gr();
}


