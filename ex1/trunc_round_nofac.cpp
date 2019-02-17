#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;

int main(){
	float el=1, sum=1, x=0, min=1.e-5;
	int n=1;
	ofstream out;
	cout << "inserire x\n";
	cin >> x; cin.get();
	
	do{
		el*=(-x)/n;
		sum+=el;
		++n;
		if(abs(el/sum)<min && sum!=0) break;
	}while(true);
	out.open("trunc_round_nofac.txt");
	out.precision(4);
	out << "x\t" << "n\t" << "sum\t" << "exp(-x)\t" << "rel" << endl;
	out << x << "\t" << n << "\t" << sum << "\t" << exp(-x) << "\t"<< (abs(sum-exp(-x)))/exp(-x) << endl;
	out.close();
	return 0;
}
