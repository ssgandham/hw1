/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sort.hh"


int binary_search(int key, keytype* A, int start, int end){
  // int low = start, high=max(start,end+1);
  //
  // i
  while(start<end){
    int mid=floor((start+end)/2);
    if(key<=A[mid]) end=mid;
    else start = mid+1;
  }

  return end;
}

void swap(int a, int b){
  int tmp = a;
  a=b;
  b=tmp;
}

void pmerge(keytype* T, int start1, int end1, int start2, int end2, keytype* A, int start3){
  int n1 = end1-start1+1;
  int n2 = end2-start2+1;

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

    #pragma omp parallel{
      pmerge(T, start1, mid1-1, start2, mid2-1, A, start3);
      pmerge(T, mid1+1, end1, mid2, end2, A, mid3+1);
    }
  }

}

void pmerge_sort(keytype* A, int start, int end, keytype* B, int index){
  int n=end-start+1;
  if(n==1) B[index] = A[start];
  else {
    keytype* T = new keytype[n];
    int split_var1 = floor((start+end)/2);
    int split_var2 = split_var1-start+1;
    #pragma omp parallel{
      pmerge_sort(A, start, split_var1, T, 0);
      pmerge_sort(A, split_var1+1, end, T, split_var2+1);
    }
    pmerge(T,0,split_var2,split_var2+1,n,B,index);
  }
}

void parallelSort(int N, keytype* A){
  keytype* B = new keytype[N];
  pmerge_sort(A, 0, N-1, B, 0);

}
