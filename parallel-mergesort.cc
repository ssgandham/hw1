/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "sort.hh"


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

int max_number(int a, int b){
  if(a>b) return a;
  return b;
}

int binary_search(int key, keytype* A, int start, int end){
  int low1 = start;
	int high1 = max_number(start, end + 1);
	int mid1;
  while(low1<high1){
    int mid1=floor((low1+high1)/2);
    if(key<=A[mid1]) high1=mid1;
    else low1 = mid1+1;
  }

  return high1;
}
void serialMergeForParallel(keytype* T, int p1, int r1, int p2, int r2, keytype* A, int p3) {

	// T[p1] is the first element of the first sub-array
	// T[r1] is the last element of the first sub-array
	// T[p2] is the first element of the second sub-array
	// T[r2] is the last element of the second sub-array

	// A[p3] is the first element where the right value should be written to

	// control variables:
	int i = p1;
	int j = p2;
	int k = p3;

	while ( (i <= r1) && (j <= r2) ) {

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
	while (i <= r1) {

		A[k] = T[i];
		i++;
		k++;

	}
	// if the second array has elements left:
	while (j <= r2) {

		A[k] = T[j];
		j++;
		k++;

	}

}

void swap(int a, int b){
  int tmp = a;
  a=b;
  b=tmp;
}

void pmerge(keytype* T, int start1, int end1, int start2, int end2, keytype* A, int start3, int index){
  int n1 = end1-start1+1;
  int n2 = end2-start2+1;

  	if (n1+n2 <= index) {
  		// serialize the merge:
  		serialMergeForParallel(T, start1, end1, start2, end2, A, start3);

  	} else {
  if(n1<n2) {
    swap(start1,start2);
    swap(end1,end2);
    swap(n1,n2);
  }

  if(n1==0) return;
  else{
    int mid1 = floor((start1+end1)/2);
    int mid2 = binary_search(T[mid1],T, start2,end2);
    int mid3 = start3+(end1-start1)+(end2-start2);

    A[mid3] = T[mid1];

    // #pragma omp parallel

    // {


    #pragma omp task
    {
      pmerge(T, start1, mid1-1, start2, mid2-1, A, start3, index);
    }
      pmerge(T, mid1+1, end1, mid2, end2, A, mid3+1, index);
      #pragma omp taskwait
    // }
  }

}
}

void pmerge_sort(keytype* A, int start, int end, keytype* B, int index){
  printf("start: %d end :%d index : %d",start,end,index);
  printf("\n");
  int n=end-start+1;

  if ( n <= index) {

		qsort(A + start, n, sizeof(keytype), compare);
		return;

	}
  else {

    //keytype* T = new keytype[n];
    int split_var1 = floor((start+end)/2);
    int split_var2 = split_var1-start+1;
    // #pragma omp parallel
    // {
    #pragma omp task
   {
      pmerge_sort(A, start, split_var1, B, index);
    }
      pmerge_sort(A, split_var1+1, end, B, index);

    #pragma omp taskwait
    // printf("\nBefore done");
    // for(int i=0;i<N;i++) {
    //   printf("A[%d] : %lu \t", i, A[i] );
    // }
    // printf("\nBefore done");
    // for(int i=0;i<N;i++) {
    //   printf("T[%d] : %lu \t", i, T[i] );
    // }
    // printf("\n");
    // parallelMerge(A, start, middle, middle + 1, end, temp, start, base);
    // pmerge(T,0,split_var2,split_var2+1,n,B,index);
    pmerge(A,start,split_var1,split_var1+1,end,B,start, index);
    memcpy(A + start, B + start, (end - start + 1) * sizeof(keytype));
  }
}

void parallelSort(int N, keytype* A){
  keytype* B = new keytype[N];
  for(int i=0;i<N;i++) {
    printf("A[%d] : %lu \t", i, A[i] );
  }
  printf("\n");

  #pragma omp parallel
{
  //int numOfThreads = omp_get_num_threads();
  #pragma omp master
  {
  // //   printf("Number of threads spawned: %d\n", numOfThreads);
  }

  #pragma omp single
  {
  pmerge_sort(A, 0, N-1, B, 2);


}
}

for(int i=0;i<N;i++) {
  printf("A[%d] : %lu \t", i, A[i] );
}
}
