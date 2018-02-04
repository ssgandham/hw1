/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sort.hh"

#include <string.h>

int number_swap(int *num1, int *num2) {
	int tmp = *num1;
	*num1 = *num2;
	*num2 = tmp;
}

int get_max_number(int numbA, int numbB) {
	if (numbA > numbB)
		return numbA;
	else
		return numbB;
}

int binary_search(int low1, int high1, keytype key, keytype* A) {

	int low = low1, high = get_max_number(low1, high1 + 1);
	while (low < high) {
		int mid = (low + high) / 2;
		if (key <= A[mid]) {
			high = mid;
		} else {
			low = mid + 1;
		}
	}
	return high;
}

void pmerge(int start1, int start2, int start3, int end1, int end2, int index,
		keytype* T, keytype* A) {
	int len1 = end1 - start1 + 1;
	int len2 = end2 - start2 + 1;

	if ((len1+len2) <= index) {

		int tmp_start1 = start1, tmp_start2 = start2, tmp_end1 = end1,
				tmp_end2 = end2, tmp_index = start3;

		while ((tmp_start1 <= tmp_end1) && (tmp_start2 <= tmp_end2)) {

			if (T[tmp_start1] <= T[tmp_start2])
				A[tmp_index++] = T[tmp_start1++];

			else
				A[tmp_index++] = T[tmp_start2++];

		}

		while (tmp_start1 <= tmp_end1)
			A[tmp_index++] = T[tmp_start1++];
		while (tmp_start2 <= tmp_end2)
			A[tmp_index++] = T[tmp_start2++];

	} else {
		if (len1 < len2) {
			number_swap(&start1, &start2);
			number_swap(&end1, &end2);
			number_swap(&len1, &len2);
		}

		if (len1 == 0) {

			return;

		} else {

			int mid1 = (start1 + end1) / 2;
			int mid2 = binary_search(start2, end2, T[mid1], T);
			int mid3 = start3 + (mid1 - start1) + (mid2 - start2);
			A[mid3] = T[mid1];

#pragma omp task
			{
				pmerge(start1, start2, start3, mid1 - 1, mid2 - 1, index, T, A);
			}

			pmerge(mid1 + 1, mid2, mid3 + 1, end1, end2, index, T, A);
#pragma omp taskwait
		}
	}

}

void pmerge_sort(int start, int end, int index, keytype* A, keytype* B) {
	int n = (end - start) + 1;
	if (n > 1) {
		int mid = (start + end) / 2;

#pragma omp task
		{
			pmerge_sort(start, mid, index, A, B);
    }
		pmerge_sort(mid + 1, end, index, A, B);

#pragma omp taskwait
		pmerge(start, mid + 1, start, mid, end, index, A, B);
		for (int i = start; i <= end; i++)
			A[i] = B[i];
	}
}

void parallelSort(int N, keytype* A) {
	keytype* B = new keytype[N];
	pmerge_sort(0, N - 1, 10, A, B);
  free(B);

}
/* eof */
