#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include "simplex.h"

int main() {
	double** A;
	double* b;
	double* c;
	A = (double**)malloc(sizeof(double*) * 100);
	for (int i = 0; i < 100; ++i) {
		A[i] = (double*)malloc(sizeof(double) * 100);
	}
	b = (double*)malloc(sizeof(double) * 100);
	c = (double*)malloc(sizeof(double) * 100);
	int n;
	int m;
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			scanf("%lf", &A[i][j]);
		}
		scanf("%lf", &b[i]);
	}
	for (int j = 0; j < m; ++j) {
		scanf("%lf", &c[j]);
	}
	Simplex LP = Simplex(n, m, A, b, c);
	LP.solve();
	printf("%d\n%lf\n", LP.msg, LP.optimal);
	for (int i = 0; i < m; ++i) {
		printf("%lf ", LP.solution[i]);
	}
	return 0;
}