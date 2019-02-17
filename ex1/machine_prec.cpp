#include<iostream>
#include<fstream>
#include<limits>
#include<typeinfo>
using namespace std;

template<typename T>
void machine_prec(){
	ofstream out;
	out.open("machine_prec.txt");
	out << "running machine_prec on " << typeid(T).name() << endl;
	out.precision(20);
	T eps=1., uno=1.;
	int it=0;
	do{
		eps=eps/2;
		uno=1.+eps;
		++it;
		out << "epsilon= " << eps << " uno= " << uno << " iterations= " << it << endl;
	}while(uno!=1);
	out << "precisione macchina per " << typeid(T).name() << " = " << numeric_limits<T>::epsilon() << endl;
}

int main(){
	machine_prec<double>();
	return 0;
}
