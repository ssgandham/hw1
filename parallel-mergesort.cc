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
// #include <omp.h>

int number_swap(int *num1, int *num2){
  int tmp  = *num1;
      *num1 = *num2;
      *num2 = tmp;
}

int compare (const void* a, const void* b)
{
  keytype ka = *(const keytype *)a;
  keytype kb = *(const keytype *)b;
  if (ka < kb)
    return -1;
  else if (ka == kb)
    return 0;
  else
    return 1;
}

void serialMergeForParallel(keytype* T, int start1, int end1, int start2, int end2, keytype* A, int start3) {

	// T[start1] is the first element of the first sub-array
	// T[end1] is the last element of the first sub-array
	// T[start2] is the first element of the second sub-array
	// T[end2] is the last element of the second sub-array

	// A[start3] is the first element where the right value should be written to

	// control variables:
	int i = start1;
	int j = start2;
	int k = start3;

	while ( (i <= end1) && (j <= end2) ) {

		if ( T[i] <= T[j] ) {

			A[k] = T[i];
			i++;

		} else {

			A[k] = T[j];
			j++;

		}

		k++;

	}

	// either array might have elements left
	// if the first array has elements left:
	while (i <= end1) {

		A[k] = T[i];
		i++;
		k++;

	}
	// if the second array has elements left:
	while (j <= end2) {

		A[k] = T[j];
		j++;
		k++;

	}

}


int get_max_number(int numbA, int numbB) {
	if(numbA>numbB) return numbA;
  else return numbB;
}

int binarySearch(keytype key, keytype* sub, int low1, int high1) {
	// sub[p] is the first element of the sub-array
	// sub[r] is the last element of the sub-array

	int low = low1, high = get_max_number(low1, high1+ 1);


	while (low < high) {

		int mid = (low + high) / 2;

		if (key <= sub[mid]) {

			high = mid;

		} else {

			low = mid + 1;

		}
	}

	return high; // returns the index such that all the elements below this index are lower than the key

}

void parallelMerge(keytype* T, int start1, int r1, int start2, int r2, keytype* A, int start3, int base) {
	// T[start1] is the first element of the first sub-array
	// T[r1] is the last element of the first sub-array
	// T[start2] is the first element of the second sub-array
	// T[r2] is the last element of the second sub-array

	// T[start1..r1] AND T[start2..r2] are sorted sub-arrays

	// A[start3] is the first element of array that holds the final value

	int n1 = r1 - start1 + 1; // number of elements in the first sub-array
	int n2 = r2 - start2 + 1; // number of elements in the second sub-array

	int N = n1 + n2;

	if (N <= base) {
		// serialize the merge:
		serialMergeForParallel(T, start1, r1, start2, r2, A, start3);

	} else {
		// divide and conquer the merge operation:

		if (n1 < n2) { // complying to our assumption
      number_swap(&start1,&start2);
      number_swap(&r1,&r2);
      number_swap(&n1,&n2);
		  // int temp;
      //
		  // // exchange start1 and start2
		  // temp = start1;
		  // start1 = start2;
		  // start2 = temp;
      //
		  // // exchange r1 and r2
		  // temp = r1;
		  // r1 = r2;
		  // r2 = temp;
      //
		  // // exchange n1 and n2
		  // temp = n1;
		  // n1 = n2;
		  // n2 = temp;

		}

		if (n1 == 0) { // if both the arrays are empty

			return;

		} else {

			int q1 = (start1 + r1) / 2;
			int q2 = binarySearch(T[q1], T, start2, r2);
			int q3 = start3 + (q1 - start1) + (q2 - start2);
			A[q3] = T[q1];

			#pragma omp task
      {
			  parallelMerge(T, start1, q1 - 1, start2, q2 - 1, A, start3, base);
      }
			parallelMerge(T, q1 + 1, r1, q2, r2, A, q3 + 1, base);
      #pragma omp taskwait
		}
	}

}

void pmerge_sort(keytype* A, int start, int end, keytype* B, int index) {

	int n = end - start + 1;

	if ( n <= index) {

		// sort the sub-array in place and return:
		qsort(A + start, n, sizeof(keytype), compare);
		return;

	} else {

		// divide up the task:

		// A[middle] is the last element of the first sub-array
		// int mid = start + (end - start) / 2; // want to avoid (start + end) /2
    int mid = (start+end)/2;

    // #pragma omp task
    // {
		  pmerge_sort(A, start, mid, B, index); // recursively divide up the first sub-array on a separate thread

    pmerge_sort(A, mid + 1, end, B, index); // recursively divide up the second sub-array on the same thread
    // }
    #pragma omp taskwait // wait for both tasks to sync up here before proceding to the merge
		//merge(A, start, middle, end, temp); // merge the sorted sub-arrays sequentially
		parallelMerge(A, start, mid, mid + 1, end, B, start, index); // merge the sorted sub-arrays parallely
    printf("copy done from : start : %d end:%d\n",start,end);
    // for(int i=start;i<=(end-start+1);i++){
    //   int tmp = B[i];
    //   B[i] = A[i];
    //   A[i] = tmp;
    // }
		memcpy(A + start, B + start, (end - start + 1) * sizeof(keytype));
	}

}

void parallelSort (int N, keytype* A) // the result goes into A
{
	keytype* B = new keytype[N]; // on heap of master, shared by all threads
//   for(int i=0;i<N;i++) {
//   printf("A[%d] : %lu \t", i, A[i] );
// }
printf("\n");
  // #pragma omp parallel
  // {
  // 	// int numOfThreads = omp_get_num_threads();
  //   #pragma omp master
  //   {
  //     // printf("Number of threads spawned: %d\n", numOfThreads);
  //   }
  //
	//   #pragma omp single
  //   {
       pmerge_sort(A, 0, N-1, B, 2);
      // mergeSort(A, 0, N-1, temp, N/4); // call the recursive mergeSort with a single thread
  //   }
  //
  // }
//   for(int i=0;i<N;i++) {
//   printf("A[%d] : %lu \t", i, A[i] );
// }
printf("\n");

}
/* eof */
