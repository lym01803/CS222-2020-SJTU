#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#define MAXN 105000
int v[MAXN];
int inline minn(int a, int b) {
	return a < b ? a : b;
}
int inline maxn(int a, int b) {
	return a > b ? a : b;
}
int main() {
	int n = 0, x;
	freopen("Data-P4.txt", "r", stdin);
	freopen("Data-P4-out.txt", "w", stdout);
	while (scanf("%d", &x) != EOF) {
		v[++n] = x;
	}
	//clock_t start_clock = clock();
	int** f = (int**)malloc(sizeof(int*) * (n + 2));
	for (int i = 0, sz = sizeof(int) * (n + 2); i < n + 2; ++i) {
		f[i] = (int*)malloc(sz);
		memset(f[i], 0, sz);
	}
	for (int i = 1; i < n; ++i) {
		f[i][i] = v[i];
		f[i][i + 1] = maxn(v[i], v[i + 1]);
	}
	f[n][n] = v[n];
	for (int k = 2; k < n; ++k) {
		for (int i = 1, j; i + k <= n; ++i) {
			j = i + k;
			f[i][j] = maxn(v[i] + minn(f[i + 2][j], f[i + 1][j - 1]), v[j] + minn(f[i][j - 2], f[i + 1][j - 1]));
		}
	}
	//clock_t end_clock = clock();
	int L = 1, R = n;
	while (L <= R) {
		if (L == R) {
			printf("%d ", v[L]);
			break;
		}
		if (L + 1 == R) {
			if (v[L] > v[R]) {
				printf("%d ", v[L]);
			}
			else {
				printf("%d ", v[R]);
			}
			break;
		}
		if (v[L] + minn(f[L + 2][R], f[L + 1][R - 1]) > v[R] + minn(f[L][R - 2], f[L + 1][R - 1])) {
			printf("%d ", v[L]);
			if (f[L + 2][R] < f[L + 1][R - 1]) {
				L += 2;
			}
			else {
				L += 1;
				R -= 1;
			}
		}
		else {
			printf("%d ", v[R]);
			if (f[L][R - 2] < f[L + 1][R - 1]) {
				R -= 2;
			}
			else {
				L += 1;
				R -= 1;
			}
		}
	}
	printf("\n%d\n", f[1][n]);
	//printf("Time: %lf second", (double)(end_clock - start_clock) / CLOCKS_PER_SEC);
}