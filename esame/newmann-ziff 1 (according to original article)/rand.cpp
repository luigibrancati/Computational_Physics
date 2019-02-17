#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
using namespace std;

int main(){
	srand(time(NULL));

	for(int i=0; i<10000; i++){
		cout << rand() << endl;		
	}
}
