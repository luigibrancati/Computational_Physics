#include<iostream>
#include<typeinfo>
#include<limits>
#include<fstream>
using namespace std;

template <typename T>
void under_over(){
	ofstream out;
	out.open("under_over.txt");
	out << "running under_over on " << typeid(T).name() << endl;
	T over=1., under=1.;
	int it=0;

	do{
		over=over*2;
		++it;
		out << "over= " << over << " iterations= " << it << endl;
	}while(over< numeric_limits<T>::infinity());
	out << "massimo per " << typeid(T).name() << " nelle librerie= " << numeric_limits<T>::max() << endl;
	it=0;
	do{
		under=under/2;
		++it;
		out << "under= " << under << " iterations= " << it << endl;
	}while(under>0);
	out << "minimo per " << typeid(T).name() << " nelle librerie= " << numeric_limits<T>::min() << endl;
}

int main(){
	under_over<float>();
	return 0;
}
