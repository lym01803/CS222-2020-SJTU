#include "simplex.h"
#include <cstdio>

Simplex::Simplex(int n, int m, double** A, double* b, double* c) {
	this->n = n;
	this->m = m;
	this->optimal = 0.0;
	this->msg = 0;
	this->eps = 1e-5;
	this->A = (double**)malloc(sizeof(double*) * n);
	for (int i = 0; i < n; ++i) {
		this->A[i] = (double*)malloc(sizeof(double) * (m + n));
		for (int j = 0; j < m; ++j) {
			this->A[i][j] = A[i][j];
		}
		for (int j = m; j < m + n; ++j) {
			this->A[i][j] = 0.0;
		}
		this->A[i][m + i] = 1.0;
	}
	this->b = (double*)malloc(sizeof(double) * n);
	for (int i = 0; i < n; ++i) {
		this->b[i] = b[i];
	}
	this->c = (double*)malloc(sizeof(double) * (m + n));
	for (int i = 0; i < m; ++i) {
		this->c[i] = c[i];
	}
	for (int i = m; i < m + n; ++i) {
		this->c[i] = 0.0;
	}
	this->d = (double*)malloc(sizeof(double));
	this->d[0] = 0.0;
	this->is_base = (int*)malloc(sizeof(int) * (m + n));
	for (int i = 0; i < m; ++i) {
		this->is_base[i] = 0;
	}
	for (int i = m; i < m + n; ++i) {
		this->is_base[i] = 1;
	}
	this->solution = (double*)malloc(sizeof(double) * m);
	for (int i = 0; i < m; ++i) {
		this->solution[i] = 0.0;
	}
}

int Simplex::find_in_base() {
	int ib = -1;
	for (int i = 0; i < this->m; ++i) {
		if (!this->is_base[i] && (ib < 0 || this->c[i] > this->c[ib])) {
			ib = i;
		}
	}
	return ib;
}

int Simplex::find_row(int col) {
	int r = -1;
	for (int i = 0; i < this->n; ++i) {
		if (this->A[i][col] > this->eps && (r < 0 || this->b[i] / this->A[i][col] < this->b[r] / this->A[r][col])) {
			r = i;
		}
	}
	return r;
}

void Simplex::remove_base(int row) {
	for (int i = 0; i < this->m; ++i) {
		if (this->is_base[i] && this->A[row][i] > this->eps) {
			this->is_base[i] = 0;
			return;
		}
	}
}

void Simplex::cancel_row(int row, int col) {
	double tmp = this->A[row][col];
	for (int i = 0; i < this->m; ++i) {
		this->A[row][i] /= tmp;
	}
	this->b[row] /= tmp;
}

void Simplex::cancel_matrix(int row, int col) {
	int i, j;
	double tmp;
	for (i = 0; i < row; ++i) {
		tmp = this->A[i][col];
		for (j = 0; j < this->m; ++j) {
			this->A[i][j] -= tmp * this->A[row][j];
		}
		this->b[i] -= tmp * this->b[row];
	}
	for (i = row + 1; i < this->n; ++i) {
		tmp = this->A[i][col];
		for (j = 0; j < this->m; ++j) {
			this->A[i][j] -= tmp * this->A[row][j];
		}
		this->b[i] -= tmp * this->b[row];
	}
	tmp = this->c[col];
	for (j = 0; j < this->m; ++j) {
		this->c[j] -= tmp * this->A[row][j];
	}
	this->d[0] -= tmp * this->b[row];
}

void Simplex::simplex() {
	int k, r;
	for (int i = 0; i < this->m; ++i) {
		if (this->is_base[i]) {
			for (int j = 0; j < this->n; ++j) {
				if (this->A[j][i] > this->eps) {
					this->cancel_matrix(j, i);
					break;
				}
			}
		}
	}
	while (1) {
		k = find_in_base();
		if (this->c[k] <= 0) {
			this->msg = 0;
			return;
		}
		r = find_row(k);
		if (r < 0) {
			this->msg = 1;
			return;
		}
		remove_base(r);
		cancel_row(r, k);
		cancel_matrix(r, k);
		this->is_base[k] = 1;
	}
}

void Simplex::solve() {
	double* tmpc = (double*)malloc(sizeof(double) * this->m);
	int i, j;
	for (i = 0; i < this->m; ++i) {
		tmpc[i] = this->c[i];
		this->c[i] = 0;
	}
	for (i = this->m; i < this->m + this->n; ++i) {
		this->c[i] = -1.0;
	}
	this->m += this->n;
	this->simplex();
	if (this->msg == 1) return;
	if (this->d[0] < -(this->eps)) {
		this->msg = 2;
		return;
	}
	this->d[0] = 0.0;
	this->m -= this->n;
	for (i = 0; i < this->m; ++i) {
		this->c[i] = tmpc[i];
	}
	this->simplex();
	if (this->msg == 1) return;
	for (i = 0; i < this->m; ++i) {
		if (this->is_base[i]) {
			for (j = 0; j < this->n; ++j) {
				if (this->A[j][i] > this->eps) {
					this->solution[i] = this->b[j];
					break;
				}
			}
		}
	}
	this->optimal = -this->d[0];
}

void Simplex::set_eps(double eps) {
	this->eps = eps;
}

void Simplex::print() {
	for (int i = 0; i < this->n; ++i) {
		for (int j = 0; j < this->m; ++j) {
			printf("%6lf ", this->A[i][j]);
		}
		printf("%6lf\n", this->b[i]);
	}
	for (int j = 0; j < this->m; ++j) {
		printf("%6lf ", this->c[j]);
	}
	printf("%6lf\n\n", this->d[0]);
	getchar();
}
