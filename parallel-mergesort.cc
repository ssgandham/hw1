/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "sort.hh"

#include <string.h>

<<<<<<< HEAD
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

void pmerge_sort(keytype* A, int start, int end, keytype* B, int index) {
	int n = (end - start) + 1;
	if (n > 1) {
		int mid = (start + end) / 2;

#pragma omp task
		{
			pmerge_sort(A, start, mid, B, index);
    }
		pmerge_sort(A, mid + 1, end, B, index);
=======

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

int binary_search(int low1, int high1, keytype key, keytype* arr) {
	// sub[p] is the first element of the sub-array
	// sub[r] is the last element of the sub-array

	int low = low1, high = get_max_number(low1, high1+ 1);


	while (low < high) {

		int mid = (low + high) / 2;

		if (key <= arr[mid]) {

			high = mid;

		} else {

			low = mid + 1;

		}
	}

	return high; // returns the index such that all the elements below this index are lower than the key

}

// void parallelMerge(keytype* T, int start1, int end1, int start2, int end2, keytype* A, int start3, int index) {
void pmerge(int start1,int start2, int start3, int end1, int end2, int index, keytype* T, keytype* A){
	// T[start1] is the first element of the first sub-array
	// T[end1] is the last element of the first sub-array
	// T[start2] is the first element of the second sub-array
	// T[end2] is the last element of the second sub-array

	// T[start1..end1] AND T[start2..end2] are sorted sub-arrays

	// A[start3] is the first element of array that holds the final value

	int n1 = end1 - start1 + 1;
	int n2 = end2 - start2 + 1;

	//int N = n1 + n2;

	// if ((n1+n2) <= index) {
	// 	// serialize the merge:
  //   // printf("n1 : %d  : n2 : %d : index : %d \n",n1,n2,index);
  //   printf("\nstart1 : %d end1 : %d start2:%d end2:%d : start3 : %d\n",start1,end1,start2,end2,start3);
  //   //qsort(A + star1, end1, sizeof(keytype), compare);
  //   // int start_tmp,end_tmp;
  //   // if(start1<end1 && start2<end2){
  //   //   start_tmp = start1>start2?start2:start1;
  //   //   end_tmp = end1>end2?end1:end2;
  //   // }else if(start1<end1){
  //   //   start_tmp=start1;
  //   //   end_tmp=end1;
  //   // }else if(start2<end2){
  //   //   start_tmp=start2;
  //   //   end_tmp=end2;
  //   // }
  //   //
  //   // start_tmp = start_tmp<0?0:start_tmp;
  //   // end_tmp = end_tmp<0?0:end_tmp;
  //   //   if(start_tmp<end_tmp)
  //   //   {
  //   //   printf("start_tmp : %d end_tmp : %d\n",start_tmp,end_tmp);
  //   //   for(int i=start3,m=start_tmp;m<end_tmp;m++,i++) A[i]=T[m];
  //   //   qsort(A+start_tmp, end_tmp, sizeof(keytype), compare);
  //   // }
  //     printf("\n\nBefore Sorting\n\n");
  //   for(int i=0;i<10;i++)
  //       printf("T[%d] : %lu \t", i, T[i] );
  //
  //       printf("\n\n");
  //       for(int i=0;i<10;i++)
  //       printf("A[%d] : %lu \t", i, A[i] );
  //
	// 	serialMergeForParallel(T, start1, end1, start2, end2, A, start3);
  //     printf("\n\nAfter Sorting\n\ns");
  //   for(int i=0;i<10;i++)
  //       printf("\T[%d] : %lu \t", i, T[i] );
  //
  //       printf("\n\n");
  //       for(int i=0;i<10;i++)
  //       printf("A[%d] : %lu \t", i, A[i] );
  //
	// } else {

{
		if (n1 < n2) {
      number_swap(&start1,&start2);
      number_swap(&end1,&end2);
      number_swap(&n1,&n2);

		}

		if (n1 == 0) {

			return;

		} else {

			int mid1 = (start1 + end1) / 2;
			int mid2 = binary_search(start2, end2,T[mid1], T);
			int mid3 = start3 + (mid1 - start1) + (mid2 - start2);
			A[mid3] = T[mid1];

			#pragma omp task
      {

			  pmerge(start1,start2, start3, mid1 - 1,mid2 - 1, index, T, A);
      }

      pmerge(mid1+1,mid2,mid3+1,end1,end2,index, T,A);
      #pragma omp taskwait
		}
	}
>>>>>>> merge_sort

#pragma omp taskwait
		pmerge(start, mid + 1, start, mid, end, index, A, B);
		for (int i = start; i <= end; i++)
			A[i] = B[i];
	}
}

<<<<<<< HEAD
void parallelSort(int N, keytype* A) {
	keytype* B = new keytype[N];
	pmerge_sort(A, 0, N - 1, B, 10);
  free(B);

=======
void pmerge_sort(keytype* A, int start, int end, keytype* B, int index) {



	if ( (end-start+1)>1) {
  //
	// 	// sort the sub-array in place and return:
	// 	qsort(A + start, n, sizeof(keytype), compare);
	// 	return;
  //
	// } else {

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
    pmerge(start, mid + 1, start, mid, end, index, A, B);
		// parallelMerge(A, start, mid, mid + 1, end, B, start, index); // merge the sorted sub-arrays parallely
    //printf("\ncopy done from : start : %d end:%d\n",start,end);
    // for(int i=start;i<=(end-start+1);i++){
    //   int tmp = B[i];
    //   B[i] = A[i];
    //   A[i] = tmp;
    // }
  //  printf("B : %lu : B+ start : %lu\n",B, B+start);
  // printf("before mem copy :start : %d : end : %d\n",start,end);
  //     for(int i=0;i<10;i++)
  //     printf("A[%d] : %lu \t", i, A[i] );
  //
  //     printf("\n\n");
  //     for(int i=0;i<10;i++)
  //     printf("B[%d] : %lu \t", i, B[i] );
      for(int i=start;i<=end;i++) A[i]=B[i];
		//memcpy(A + start, B + start, (end - start + 1) * sizeof(keytype));
    // printf("\n");
    // printf("after mem copy :start : %d : end : %d\n",start,end);
    // printf("\n\n");
    // for(int i=0;i<10;i++)
    // printf("A[%d] : %lu \t", i, A[i] );
    //
    // printf("\n\n");
    // for(int i=0;i<10;i++)
    // printf("B[%d] : %lu \t", i, B[i] );
	}

}

void parallelSort (int N, keytype* A) // the result goes into A
{
<<<<<<< HEAD
	keytype* B = new keytype[N]; 
	#pragma omp parallel
	#pragma omp single nowait
	pmerge_sort(A, 0, N-1, B, 10);
      
=======
	keytype* B = new keytype[N]; // on heap of master, shared by all threads
//   for(int i=0;i<N;i++) {
//   printf("A[%d] : %lu \t", i, A[i] );
// }
// printf("\n");
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
// printf("\n");

>>>>>>> merge_sort
>>>>>>> c681d7ac5a39e0c47f27bf6e8cdb5b9af529bc65
}
/* eof */
