#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define MAXN 1050000
int inline Abs(const int x) {
	return x > 0 ? x : -x;
}
int inline Minn(const int a, const int b) {
	return a < b ? a : b;
}
int number[MAXN];
int main() {
	freopen("Data-P1.txt", "r", stdin);
	//freopen("P1-out.txt", "w", stdout);
	int n = 0;
	int S_pos = 0;
	int S_neg = 0;
	int x;
	while (scanf("%d", &x) != EOF) {
		number[n++] = x;
		S_pos = x > 0 ? (S_pos + x) : S_pos;
		S_neg = x < 0 ? (S_neg + x) : S_neg;
	}
	int* f = (int*)malloc(sizeof(int) * (S_pos - S_neg + 1));
	memset(f, 0, sizeof(int) * (S_pos - S_neg + 1));
	f[-S_neg] = 1;
	for (int i = 0; i < n; ++i) {
		if (number[i] < 0) {
			for (int s = 0; s <= S_pos - S_neg + number[i]; ++s) {
				f[s] |= f[s - number[i]];
			}
		}
		if (number[i] > 0) {
			for (int s = S_pos - S_neg; s >= number[i]; --s) {
				f[s] |= f[s - number[i]];
			}
		}
	}
	int minn = 0x7fffffff;
	for (int i = 0; i <= S_pos - S_neg; ++i) {
		if (f[i]) {
			minn = Minn(minn, Abs(S_pos - S_neg - (i << 1)));
		}
	}
	printf("%d\n", minn);
	return 0;
}