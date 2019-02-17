#include<iostream>
#include<limits>
using namespace std;

int pow(int base, int exp){
	if(exp==0) return 1;
	else for(int i=0; i<exp; i++) base*=base;
	return base;
}

int main(){
	int under=-1, over=1;
	int exp=1;

	//overflow
	do{
		over=pow(2,exp)-1;
		cout<< "exp= " << exp << " " << "2^exp-1= " << over << endl;
		if (over<=0) break;
		over=pow(2,exp);
		cout<< "exp= " << exp << " " << "2^exp= " << over << endl;
		if (over<=0) break;
		++exp;
	}while(true);
	cout << "massimo per int nelle librerie= " << numeric_limits<int>::max() << endl;

	//underflow
	exp=1;
	do{
		under=-pow(2,exp)+1;
		cout<< "exp= " << exp << " " << "-2^exp+1= " << under << endl;
		if (under>=0) break;
		under=-pow(2,exp);
		cout<< "exp= " << exp << " " << "-2^exp= " << under << endl;
		if (under>=0) break;
		++exp;
	}while(true);
	cout << "minimo per int nelle librerie= " << numeric_limits<int>::min() << endl;
	return 0;
}



