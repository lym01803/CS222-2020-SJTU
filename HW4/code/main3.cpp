#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define MAXN 1005000
/*
cost(a, b). a, b: {A,T,G,C,-} --> {1,2,3,4,0}
*/
const int cost[5][5] = {
	{0, 1, 2, 1, 3},
	{1, 0, 1, 5, 1},
	{2, 1, 0, 9, 1},
	{1, 5, 9, 0, 1},
	{3, 1, 1, 1, 0}
};
int array_size;
int* A;
int* B;
int* C;
int* pth_in;
int* pth_out;
char* s;
char* t;
char* s_1;
char* t_1;
int len_s1;
int len_t1;
int inline minn(const int a, const int b) {
	return a < b ? a : b;
}
void Find_length_1(int left, int right, int top, int down, int** f, int** g) {
	memset(*f, 0, array_size);
	memset(*g, 0, array_size);
	// F[left][top] = 0
	for (int i = top + 1; i <= down; ++i) {
		(*f)[i] = (*f)[i - 1] + cost[0][t[i]];
	}
	int* tmp;
	for (int i = left + 1; i <= right; ++i) {
		(*g)[top] = (*f)[top] + cost[s[i]][0];
		for (int j = top + 1; j <= down; ++j) {
			(*g)[j] = minn(minn((*f)[j - 1] + cost[s[i]][t[j]], (*g)[j - 1] + cost[0][t[j]]), (*f)[j] + cost[s[i]][0]);
		}
		tmp = *f;
		*f = *g;
		*g = tmp;
	}
}
void Find_length_2(int left, int right, int top, int down, int** f, int** g) {
	memset(*f, 0, array_size);
	memset(*g, 0, array_size);
	//F[right][down] = 0
	for (int i = down - 1; i >= top; --i) {
		(*f)[i] = (*f)[i + 1] + cost[0][t[i+1]];
	}
	int* tmp;
	for (int i = right - 1; i >= left; --i) {
		(*g)[down] = (*f)[down] + cost[s[i+1]][0];
		for (int j = down - 1; j >= top; --j) {
			(*g)[j] = minn(minn((*f)[j + 1] + cost[s[i + 1]][t[j + 1]], (*g)[j + 1] + cost[0][t[j + 1]]), (*f)[j] + cost[s[i + 1]][0]);
		}
		tmp = *f;
		*f = *g;
		*g = tmp;
	}
}
void Find_shortest_pth(int left, int right, int top, int down) {
	if (left == right) {
		pth_in[left] = top;
		pth_out[left] = down;
		return;
	}
	int mid = (left + right) >> 1;
	Find_length_1(left, mid, top, down, &A, &B);
	Find_length_2(mid + 1, right, top, down, &B, &C);
	int minn1 = 0x3f3f3f3f, minn2 = 0x3f3f3f3f;
	int idx1 = top, idx2 = top;
	for (int i = top, m1, m2; i < down; ++i) {
		m1 = A[i] + B[i] + cost[s[mid + 1]][0];
		m2 = A[i] + B[i + 1] + cost[s[mid + 1]][t[i + 1]];
		if (m1 < minn1) {
			minn1 = m1;
			idx1 = i;
		}
		if (m2 < minn2) {
			minn2 = m2;
			idx2 = i;
		}
	}
	int m1 = A[down] + B[down] + cost[s[mid + 1]][0];
	if (m1 < minn1) {
		minn1 = m1;
		idx1 = down;
	}
	if (minn1 < minn2) {
		Find_shortest_pth(left, mid, top, idx1);
		Find_shortest_pth(mid + 1, right, idx1, down);
	}
	else {
		Find_shortest_pth(left, mid, top, idx2);
		Find_shortest_pth(mid + 1, right, idx2 + 1, down);
	}
}
char char_to_num(char s) {
	if (s == 'A') {
		return 1;
	}
	if (s == 'T') {
		return 2;
	}
	if (s == 'G') {
		return 3;
	}
	if (s == 'C') {
		return 4;
	}
	throw s;
}
char num_to_char[5] = { '-', 'A', 'T', 'G', 'C' };
int main() {
	s = (char*)malloc(sizeof(char) * MAXN);
	t = (char*)malloc(sizeof(char) * MAXN);
	s_1 = (char*)malloc(sizeof(char) * MAXN);
	t_1 = (char*)malloc(sizeof(char) * MAXN);
	len_s1 = len_t1 = 0;
	FILE* fin;
	fin = fopen("Data-P3a.txt", "r");
	fscanf(fin, "%s", s + 1);
	fclose(fin);
	fin = fopen("Data-P3b.txt", "r");
	fscanf(fin, "%s", t + 1);
	fclose(fin);
	s[0] = t[0] = 0x3f;
	int n = 1, m = 1;
	for (; s[n] != '\0'; ++n) {
		s[n] = char_to_num(s[n]);
	}
	for (; t[m] != '\0'; ++m) {
		t[m] = char_to_num(t[m]);
	}
	//printf("%d %d\n", n, m);
	array_size = sizeof(int) * (m + 1);
	A = (int*)malloc(array_size);
	B = (int*)malloc(array_size);
	C = (int*)malloc(array_size);
	pth_in = (int*)malloc(sizeof(int) * (n + 1));
	pth_out = (int*)malloc(sizeof(int) * (n + 1));
	Find_shortest_pth(0, n - 1, 0, m - 1);
	int dis = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = pth_in[i]; j < pth_out[i]; ++j) {
			s_1[len_s1++] = '-'; //num_to_char[t[j + 1]];
			t_1[len_t1++] = num_to_char[t[j + 1]];
			dis += cost[0][t[j + 1]];
		}
		if (i < n - 1) {
			if (pth_out[i] == pth_in[i + 1]) {
				t_1[len_t1++] = '-'; // num_to_char[s[i + 1]];
				s_1[len_s1++] = num_to_char[s[i + 1]];
				dis += cost[s[i + 1]][0];
			}
			else {
				s_1[len_s1++] = num_to_char[s[i + 1]];
				t_1[len_t1++] = num_to_char[t[pth_in[i + 1]]];
				dis += cost[s[i + 1]][t[pth_in[i + 1]]];
			}
		}
	}
	s_1[len_s1] = t_1[len_t1] = '\0';
	FILE* fout;
	fout = fopen("Data-P3-out-a.txt", "w");
	fprintf(fout, "%s", s_1);
	fclose(fout);
	fout = fopen("Data-P3-out-b.txt", "w");
	fprintf(fout, "%s", t_1);
	fclose(fout);
	printf("%d\n", dis);
	return 0;
}