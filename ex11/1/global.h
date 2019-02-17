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

int N, nmcs=10000, accept, g_count;
double Lx, Ly, p, dxmax=0.1, dymax=0.1, delta=0.1;

int nbins;
double r_max;
vector<double> g; 

class particle{
public:
	particle(double a, double b): x(a), y(b) {}
	double x;
	double y;
};

vector<particle> sistem;

//stampa in file le posizioni delle particelle
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

//salva la configurazione finale
void save_config(){
	fstream out("particle_config.txt", ios::out);
	out << Lx << endl << Ly << endl << N <<endl;
	for(int i=0; i<N; i++) out << sistem[i].x << " " << sistem[i].y << endl;
	cout << "saved\n";
	out.close();	
}

//carica come configurazione iniziale l'ultima configurazione finale salvata in "particle_config.txt" (ultima della run precedente) 
void load_configuration(){
	double pold, ratio;
	cout << "loading\n";
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

//inizializzazione
void starting(){
	time_t seed=time(NULL);
	srand(seed);	
	cout << seed << endl;

	accept=0;
	//alloco le particelle in un lattice triangolare e inizializzo tutti i valori
	char choose;
	do{	
		cout << "do you want to use the previous run as a starting point? (y/n)";
		cin >> choose;
		if(choose=='n'){
			cout << "Lx: "; cin >> Lx;
			cout << "Ly: "; cin >> Ly;
			triangular_lattice();
			break;
		}
		else if(choose=='y'){
			load_configuration();
			break;
		}
		else cout << "error! invalid character, please choose y/n\n";
	}while(true);	
	
	r_max=min((double) Lx/2, (double) Ly/2);
	nbins=(r_max/delta);
	g.resize(nbins, 0);
	g_count=0;
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
void overlap(int r, double xtrial, double ytrial){
	double dr=0, dx=0, dy=0;	
	for(int i=0; i<N; i++){
		if(i!=r){
			dx=sistem[i].x-xtrial;
			dy=sistem[i].y-ytrial;
			dist(dx, dy);
			dr=dx*dx+dy*dy;
			if(dr<1){ 
				//cout << "non accettata\n";
				return;
			}
		}
	}
	
	sistem[r].x=xtrial;
	sistem[r].y=ytrial;
	accept+=1;
}

//singolo passo di montecarlo: seleziona N volte una tra le N particelle randomicamente e la muove, sistema la posizione con le pbc e controlla l'overlap
void move(){
	int r;
	double xtrial, ytrial, xold, yold, rand1, rand2;
	for(int i=0; i<N; i++){//muove N volte le N particelle a caso
		r=rand()%N;

		xold=sistem[r].x;
		yold=sistem[r].y;

		rand1=(double) rand()/RAND_MAX;
		rand2=(double) rand()/RAND_MAX;
		xtrial=xold + (2*rand1-1)*dxmax;
		ytrial=yold + (2*rand2-1)*dymax;
		PBC(xtrial, ytrial);
		overlap(r, xtrial, ytrial);
	}
}
//termalizzazione
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

//calcola g(r) finale
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

void MCHD(ofstream& part_pos){
	for(int i=1; i<nmcs; i++){
		move();
		accumulate_gr();
		if(i%50==0 && i<nmcs-1)	print_particles_positions(sistem, part_pos, i);	
	}
	print_particles_positions(sistem, part_pos, nmcs);//stampo l'ultima config, che potrebbe non rientrare nella condizione i%50==0 dell'if
	calculate_gr(); //calcolo g(r)	
}


