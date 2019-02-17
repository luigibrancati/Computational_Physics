#include<iostream>
#include<fstream>
#include<ctime>
#include<cstdlib>
using namespace std;

int main(){
	int num=100000, r;
	int seed=time(NULL);
	srand(seed);
	for(int i=0; i<num; i++){
		r=rand();
		cout << r << endl;
	}
}
