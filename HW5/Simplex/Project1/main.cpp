#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#define eps (1e-5)
#define SIMPLEX_SOLVED 1
#define SIMPLEX_UNBOUNDED 2

void init_base(int n, int m, int* is_base) {
	int i;
	for (i = 0; i < m; ++i) {
		is_base[i] = 0;
	}
	for (i = m; i < n + m; ++i) {
		is_base[i] = 1;
	}
}

int find_in_base(int n, int m, double* c, int* is_base) {
	int i;
	int ib = -1;
	for (i = 0; i < n + m; ++i) {
		if (!is_base[i] && (ib < 0 || c[i] > c[ib])) {
			ib = i;
		}
	}
	return ib;
}

int find_row(int n, int k, double** A, double* b) {
	int i;
	int r = -1;
	for (i = 0; i < n; ++i) {
		if (A[i][k] > eps && (r < 0 || b[i] / A[i][k] < b[r] / A[r][k])) {
			r = i;
		}
	}
	return r;
}

void remove_base(int n, int m, int r, double** A, int* is_base) {
	int i;
	for (i = 0; i < n + m; ++i) {
		if (is_base[i] && A[r][i] > eps) {
			is_base[i] = 0;
			return;
		}
	}
}

void cancel_row(int n, int m, int r, int k, double** A, double* b) {
	int i;
	double tmp = A[r][k];
	for (i = 0; i < n + m; ++i) {
		A[r][i] /= tmp;
	}
	b[r] /= tmp;
}

void cancel_matrix(int n, int m, int r, int k, double** A, double* b, double* c, double* d) {
	int i;
	int j;
	double tmp;
	for (i = 0; i < r; ++i) {
		tmp = A[i][k];
		for(j = 0; j < n + m; ++j){
			A[i][j] -= tmp * A[r][j];
		}
		b[i] -= tmp * b[r];
	}
	for (i = r + 1; i < n; ++i) {
		tmp = A[i][k];
		for (j = 0; j < n + m; ++j) {
			A[i][j] -= tmp * A[r][j];
		}
		b[i] -= tmp * b[r];
	}
	tmp = c[k];
	for (j = 0; j < n + m; ++j) {
		c[j] -= tmp * A[r][j];
	}
	d[0] -= tmp * b[r];
}

int Simplex(int n, int m, double** A, double* b, double* c, double* d, int* is_base) {
	int k, r;
	init_base(n, m, is_base);
	while (1) {
		k = find_in_base(n, m, c, is_base);
		if (c[k] <= 0) {
			return SIMPLEX_SOLVED;
		}
		r = find_row(n, k, A, b);
		if (r < 0) {
			return SIMPLEX_UNBOUNDED;
		}
		remove_base(n, m, r, A, is_base);
		cancel_row(n, m, r, k, A, b);
		cancel_matrix(n, m, r, k, A, b, c, d);
		is_base[k] = 1;
	}
}

void get_solution(int n, int m, double** A, double* b, double* c, double* d, int* is_base, double* solution, double* opt_value) {
	int i;
	int j;
	for (i = 0; i < n + m; ++i) {
		if (is_base[i]) {
			for (j = 0; j < n; ++j) {
				if (A[j][i] > eps) {
					solution[i] = b[j];
					break;
				}
			}
		}
		else {
			solution[i] = 0.0;
		}
	}
	opt_value[0] = -d[0];
}

double** A;
double b[2] = { 2, 2 };
double c[4] = { 1, 3, 0, 0 };
double d[1] = { 0 };
int base[4] = { 0, 0, 0, 0 };
double solution[4] = { 0, 0, 0, 0 };
double opt_value;
int main() {
	A = (double**)malloc(sizeof(double*) * 2);
	for (int i = 0; i < 2; ++i) {
		A[i] = (double*)malloc(sizeof(double) * 4);
	}
	A[0][0] = 1.0;
	A[0][1] = 1.0;
	A[0][2] = 1.0;
	A[0][3] = 0.0;
	A[1][0] = 1.0;
	A[1][1] = 1.0;
	A[1][2] = 0.0;
	A[1][3] = -1.0;
	int msg;
	msg = Simplex(2, 4 - 2, (double**)A, b, c, d, base);
	printf("%d\n", msg);
	get_solution(2, 4 - 2, (double**)A, b, c, d, base, solution, &opt_value);
	for (int i = 0; i < 4; ++i) {
		printf("%lf ", solution[i]);
	}
	printf("\n%lf\n", opt_value);
	return 0;
}