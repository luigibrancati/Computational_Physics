#include</home/luigi/Fisica_computazionale/ex2/1/lcmf.h>
using namespace std;

int main(){
	int terms, m, a, old, c;
	cout<<"seed: "; cin>>old;
	cout<<"terms: ";cin>>terms;
	cout<<"mod: ";cin>>m;
	cout<<"a: ";cin>>a;
	cout<<"c: ";cin>>c;
	
	float * r=new float[terms];
	int * x=new int[terms];

	lcmf(old, terms, m, a, c,x);
	ofstream out("randomness.txt");
	if(out.is_open()){
		out <<"#points\n";
		for(int i=0; i<terms; i++) r[i]=((float)x[i])/m;
		for(int i=0; i<(terms-1); i++) out <<r[i]<<" "<<r[i+1]<<endl;
	
		float delta=0.1;
		int n=1/delta;
		int checksum=0;
		int * h=new int[n];
		for(int i=0; i<terms; i++) h[(int)(r[i]/delta)]+=1;

		out << "\n\n#histogram\n";
		for(int i=0; i<n; i++){
			checksum+=h[i];
			out << h[i] <<endl;
		}
		out << "\n#checksum="<< checksum<<endl;
	out.close();	
	}
	else cout <<"file not opened\n";
	return 0;
}
