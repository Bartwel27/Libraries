#include <iostream>

// prototyped function

// declairations
	// arithmetic prototyped functions
	int add(int a, int b);
	int sub(int a, int b);
	int mult(int a, int b);
	int mod(int a, int b);
	double div(double a, double b);

	// bitwise prototyped functions
	int _and(int a, int b);
	int _or(int a, int b);
	int _xor(int a, int b);
	int _not(int a);
	int _leftshift(int a, int b);
	int _rightshift(int a, int b);


int main() {
	std::cout << _rightshift(10,2) << std::endl;
}

// definations 
int add(int a, int b){ return a + b; }
int sub(int a, int b){ return a - b; }
int mult(int a, int b){ return a * b; }
int mod(int a, int b){ return a % b; }
double div(double a, double b){ return a / b; }
int _and(int a, int b){ return (a & b); }
int _or(int a, int b){ return (a | b); }
int _xor(int a, int b){ return (a ^ b); }
int _not(int a){ return (~a + 1); }
int _leftshift(int a, int b){ return (a << b); }
int _rightshift(int a, int b){ return (a >> b); }
