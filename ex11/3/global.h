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

int N, nmcs=10000, nequil=500, accept;
double Lx, Ly, p, dxmax=0.2, dymax=0.2, delta=0.1, dt=0.01, T, E, E_mean=0, E_var=0;

int nbins;
double r_max;
vector<double> g; 

class potential{
public:	
	double operator()(double r){return 4*(pow((double)1/r, 12)-pow((double)1/r,6));}
}u;

class particle{
public:
	particle(double a, double b, double c, double d, double e, double f): x(a), y(b), vx(c), vy(d), ax(e), ay(f) {}
	double x;
	double y;
	double vx;
	double vy;
	double ax;
	double ay;
};

vector<particle> sistem;

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
		}
	}
}

void force(double dx,double dy,double& fx,double& fy){
	double F, r2, r6;
	r2=dx*dx+dy*dy;
	r2=1/r2;
	r6=r2*r2*r2;
	F=24*r6*(2*r6-1)*r2; //modulo della forza, da moltiplicare poi per il vettore distanza r tra le due particelle
	fx=F*dx;//componente x della forza
	fy=F*dy;//componente y della forza
	pot=4*r6*(r6-1);//energia potenziale
}

void acceleration(){
	double fx=0, fy=0, dx, dy;
	for(int i=0; i<N; i++){
		sistem[i].ax=0;
		sistem[i].ay=0;
	}
	for(int i=0; i<N-1; i++){
		for(int j=i+1; j<N; j++){
			dx=sistem[i].x-sistem[j].x;
			dy=sistem[i].y-sistem[j].y;
			dist(dx, dy);			
			force(dx, dy, fx, fy);
			sistem[i].ax+=fx;
			sistem[i].ay+=fy;
			sistem[j].ax-=fx;
			sistem[j].ay-=fy;
		}
	}
}

void Verlet(){
	for(int i=0; i<N; i++){
		sistem[i].x+=sistem[i].vx*dt+sistem[i].ax*0.5*dt;
		sistem[i].y+=sistem[i].vy*dt+sistem[i].ay*0.5*dt;
		PBC(sistem[i].x, sistem[i].y);
		sistem[i].vx+=0.5*sistem[i].ax*dt;
		sistem[i].vy+=0.5*sistem[i].ay*dt;
	}	
	acceleration();
	for(int i=0; i<N; i++){
		sistem[i].vx+=0.5*sistem[i].ax*dt;
		sistem[i].vy+=0.5*sistem[i].ay*dt;
	}

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
			if (bin<nbins) g.at(bin)+=1;
		}
	}
	
}

void calculate_gr(ofstream& out){
	double normalization=2/(N*nmcs*p);
	double shell_area, r;	
	for(int i=0; i<nbins; i++){
		r=i*delta;
		shell_area=M_PI*((r+delta)*(r+delta) - r*r);
		g[i]=g[i]*normalization/shell_area;
		out << i << "\t" << setw(W) << g[i] << endl;	
	}
}

void save_config(){
	fstream out("particle_config.txt", ios::out);
	out << Lx << endl << Ly << endl << N <<endl;
	for(int i=0; i<N; i++) out << sistem[i].x << " " << sistem[i].y << "  " << sistem[i].vx << " " << sistem[i].vy << endl;
	out.close();	
}

void load_configuration(){
	double pold, ratio;
	fstream in("particle_config.txt", ios::in);
	if(in.is_open()){
		in >> Lx;
		in >> Ly;
		in >> N;
		pold=N/(Lx*Ly);
		sistem.resize(N, particle(0,0));

		cout << "p: " << pold << "  nuova densità: "; cin >> p;
		ratio=p/pold;
		cout << ratio << endl;

		for(int i=0; i<N; i++){
				in >> sistem[i].x;
				in >> sistem[i].y;
				in >> sistem[i].vx;
				in >> sistem[i].vy;
		}
	}
	else{
		cout << "error: file not opened\n";
		return;	
	}
	in.close();

	//riscalo in base alla nuova densità
	Lx=sqrt(1/ratio)*Lx;
	Ly=sqrt(1/ratio)*Ly;
	for(int i=0; i<N; i++){
		sistem[i].x=sqrt(1/ratio)*sistem[i].x;
		sistem[i].y=sqrt(1/ratio)*sistem[i].y;
	}
}

//sistema le velocità iniziali in modo che siano in accordo col teorema di equipartizione dell'energia
void starting_velocities(){
	double vxsum=0, vysum=0, v2=0, E_k, rescale;
	//assegno velocità random
	for(int i=0; i<N; i++){
		sistem[i].vx=2*((double) rand()/RAND_MAX) - 1;
		sistem[i].vy=2*((double) rand()/RAND_MAX) - 1;
		vxsum+=sistem[i].vx;
		vysum+=sistem[i].vy;
	}
	//traslo le velocità in modo da avere vxsum e vysum (velocità del centro di massa) uguali a 0
	vxsum=vxsum/N;
	vysum=vysum/N;
	for(int i=0; i<N; i++){
		sistem[i].vx-=vxsum;
		sistem[i].vy-=vysum;
		v2+=sistem[i].vx*sistem[i].vx+sistem[i].vy*sistem[i].vy;
	}
	
	//riscalo le velocità in modo da avere l'energia cinetica desiderata
	E_k=v2*0.5/N;
	rescale=sqrt(E/T);
	for(int i=0; i<N; i++){
		sistem[i].vx*=rescale;
		sistem[i].vy*=rescale;
	}
}

void triangular_lattice(){
	do{	
		cout << "p (max: 1.1547): "; cin >> p;
		if (p>p_max) cout << "attenzione! p > p_max=1.1547! scegliere p minore\n";
	}while(p>p_max);	
	N=p*Lx*Ly;
	cout << "N: " << N << endl;
	sistem.resize(N, particle(0,0,0,0,0,0));
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

void starting(){
	time_t seed=time(NULL);
	srand(seed);	
	cout << seed << endl;

	accept=0;
	//alloco le particelle in un lattice triangolare e inizializzo tutti i valori
	char choose;
	cout << "do you want to use the previous run as a starting point? (y/n)";
	cin >> choose;
	if(choose=='n'){
		cout << "Lx: "; cin >> Lx;
		cout << "Ly: "; cin >> Ly;	
		cout << "T: "; cin >> T;
		triangular_lattice();
	}
	else load_configuration();

	r_max=min((double) Lx/2, (double) Ly/2);
	nbins=(r_max/delta);
	g.resize(nbins, 0);

	//calcolo l'energia iniziale
	E=0;
	starting_velocities();
	E=N*T;
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

void print_particles_positions(vector<particle> par, ostream& out, int time){
	out << "\n\n#step: " << time << endl;
	for(int i=0; i<par.size(); i++) out << i << "\t" << setw(W) << par[i].x << "\t" << setw(W) << par[i].y << endl;
}


