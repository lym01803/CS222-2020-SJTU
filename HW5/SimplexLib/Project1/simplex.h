#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <cstdlib>
#include <cmath>

class Simplex {
private:
	double** A;
	double* b;
	double* c;
	double* d;
	int n;
	int m;
	int* is_base;
	double eps;
	int find_in_base();
	int find_row(int col);
	void remove_base(int row);
	void cancel_row(int row, int col);
	void cancel_matrix(int row, int col);
	void simplex();
	void print();
	
public:
	int msg;
	double* solution;
	double optimal;
	Simplex(int n, int m, double** A, double* b, double* c);
	void solve();
	void set_eps(double eps);
};

#endif