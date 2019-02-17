#include<iostream>
#include<fstream>
#include<cmath>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<iomanip>
using namespace std;

typedef vector<double> vec;

void raindrop(float, float, float, float);
int choose_dir(float, float, float, float);

int main(){
	float p_up=0.1, p_down=0.6, p_left=0.15, p_right=0.15;
	
	/*do{
		cout<<"p up: "; cin >>p_up;
		cout<<"p down: "; cin >>p_down;
		cout<<"p left: "; cin >>p_left;	
		cout<<"p right: "; cin >>p_right;
		
		if((p_up+p_down+p_left+p_right)==1) break;
		else cout << "errore\n"; 		

	}while(true);*/
	srand(time(NULL));	

	raindrop(p_up, p_down, p_left, p_right);	

	return 0;
}

//NORD,SUD,EST,OVEST
void raindrop(float p_up, float p_down, float p_left, float p_right){
	double x=0, y=0, hstart=0, m_displ=0, m_displ_2=0, m_displ_2_d=0;
	int walkers=10000, t=0, t_m=0;//time
	
	//cout <<"walkers: "; cin>>walkers;
	cout<<"start height: "; cin>>hstart;

	ofstream out("raindrop.csv");
	for(int w=0; w<walkers; w++){
		out<<"\n\n#walker: "<<w<<endl<<"#time\tx\ty\n";
		x=0; y=hstart; t=0; m_displ=0;
		do{
			
			switch(choose_dir(p_up, p_down, p_left, p_right)){
				case(0): x+=1; break; //right
				case(1): y-=1; break; //down
				case(2): x-=1; break; //left
				case(3): y+=1; break; //up
			}
		
			out<< t << "\t" << x <<"\t" << y <<endl;

			t+=1;
		}while(y!=0);
		t_m+=t;
		m_displ+=x;
		m_displ_2+=x*x;
	}
	out.close();

	ofstream out1("times.csv", ios::app);

	t_m=(float)t_m/walkers;
	m_displ=(float) m_displ/walkers;
	m_displ_2=(float) m_displ_2/walkers;
	m_displ_2_d=m_displ_2-(m_displ*m_displ);	

	out1 << hstart<<"\t"<<(float)t_m/walkers <<"\t"<<m_displ<<"\t"<<m_displ_2<<"\t"<<m_displ_2_d<<endl;
	out1.close();
}

int choose_dir(float p_up, float p_down, float p_left, float p_right){
	float ran=(float) rand()/RAND_MAX;
	
	if(ran < p_right) return 0;
	else if (ran>p_right && ran<(p_right+p_down)) return 1;
	else if (ran>(p_right+p_down) && ran<(p_right+p_down+p_left)) return 2;
	else return 3;
}

