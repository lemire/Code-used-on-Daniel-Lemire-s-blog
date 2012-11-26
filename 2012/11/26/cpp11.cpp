#include <string>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

// returns the greater common divisor
constexpr int gcd(int x, int y) {
    return (x % y) == 0 ? y :  gcd(y,x % y);
}


// because gcd is constexpr, the compiler
// can compute it at compile time.
enum {GCD4AND12 = gcd(4,12)};

int main() {
	// initialize elegantly:
	const map<string,int> m = {{"Joe",2},{"Jack",3}};
	// auto can save a lot of typing:
	for(auto i : m) 
		cout << i.first << ":" << i.second << endl;
	cout << GCD4AND12 <<endl;
	
	vector<int> x = {1,2,3,4};
	for(auto i : x)
	  cout<< i <<endl;
	
	vector<vector<int> > V;
	for(int k = 0; k < 100000; ++k) {
		vector<int> x(gcd(1000,100000));
		V.push_back(x);
	}	
}