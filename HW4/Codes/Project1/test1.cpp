#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
void Print(void** A, int n) {
	for (int i = 0; i < n; ++i) {
		printf("%p ", A[i]);
	}
}
void Append(void** list, void* item, int idx) {
	*(list + idx) = item;
}
void* Get(void** list, int idx) {
	return *(list + idx);
}
int main() {
	void** temp = (void**)malloc(sizeof(void*) * 2);
	int num = 2;
	Append(temp, temp, 0);
	Append(temp, &num, 1);
	for (int i = 0; i < 10; ++i) {
		Print(temp, 2);
		printf("%d\n", *(int*)Get(temp, 1));
		temp = (void**)Get(temp, 0);
	}
	return 0;
}