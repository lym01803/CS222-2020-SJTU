#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
int inline minn(int a, int b) {
	return a < b ? a : b;
}
#define Inf (0x3f3f3f3f)
int main() {
	freopen("Data-P2.txt", "r", stdin);
	int n;
	scanf("%d", &n);
	int* f = (int*)malloc(sizeof(int) * (2 * n + 100));
	memset(f, 0x3f, sizeof(int) * (2 * n + 100));
	f[0] = 0;
	for (int i = 0, w, t; i < n; ++i) {
		scanf("%d%d", &t, &w);
		for (int T = 2 * n + 10; T >= t; --T) {
			f[T] = minn(Inf, minn(f[T - t], f[T] + w));
		}
		for (int T = t - 1; T >= 0; --T) {
			f[T] = minn(Inf, f[T] + w);
		}
	}
	for (int T = 0; T < 2 * n + 10; ++T) {
		if (f[T] <= T) {
			printf("%d\n", T);
			break;
		}
	}
	return 0;
}