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

int binary_search(int key, keytype* B, int start, int end ){
  int low = start;
  int high = max(start,end+1);
  while(low<high){
    int mid = floor((low+high)/2);
    if(key<=B[mid]) high = mid;
    else low=mid+1;
  }

  return high;
}
static int compare (keytype a, keytype b)
{
  if (a < b)
    return -1;
  else if (a == b)
    return 0;
  else
    return 1;
}



void merge(keytype* A,int Z,int mid,int N){

	int i,j,k;
	int a1 = mid - Z+1;
	int a2 = N - mid;
	keytype arr1[a1];
	keytype arr2[a2];

  for(int i=0;i<a1;i++) arr1[i]=0;
  for(int i=0;i<a2;i++) arr2[i]=0;
  // printf("\n");
  // printf("Z: %d mid :%d  N:%d",Z,mid, N);
  // printf("\n");
	for(i =0;i<a1;i++){
    // printf("i : %d arr1[i] : %d A[Z+i] :%lu",arr1[i],A[Z+i]);
    //   printf("\n");
		arr1[i]= A[Z+i];
    // printf("i : %d arr1[i] : %d A[Z+i] :%lu",arr1[i],A[Z+i]);
    //   printf("\n");
	}
  //printf("\n");
	for(j =0;j<a2;j++){
    // printf("i : %d arr1[i] : %d A[mid+1+j] :%lu",arr2[j],A[mid+1+j]);
    //   printf("\n");
    arr2[j]= A[mid+1+j];
    // printf("i : %d arr1[i] : %d A[mid+1+j] :%lu",arr2[j],A[mid+1+j]);
    //   printf("\n");
  }

	i = 0;
   	j = 0;
    	k = Z;
    	while (i < a1 && j < a2)
    	{
        	if (compare(arr1[i],arr2[j]) <=0)
        	{
            		A[k] = arr1[i];
            		i++;
        	}
        	else
        	{
            		A[k] = arr2[j];
            		j++;
        	}
        	k++;
    	}


    	while (i < a1)
    	{
        	A[k] = arr1[i];
        	i++;
        	k++;
    	}


    	while (j < a2)
    	{
        	A[k] = arr2[j];
        	j++;
        	k++;
    	}
}



void mergesort(keytype* A,int Z,int N){
	if(Z<N){
		//int mid = floor((Z+N)/2);
    // printf("\n");
    //  printf("Z : %d+ N : %d",Z,N);
		int mid = Z+(N-Z)/2;
		//mergesort(A,Z,mid-1);
		//mergesort(A,mid,N-1);

		mergesort(A,Z,mid);
    mergesort(A,mid+1,N);

		merge(A,Z,mid,N);
	}

}

void
parallelSort (int N, keytype* A)
{
  /* Lucky you, you get to start from scratch */
  //for(int i=0;i<N;i++) A[i]=0;
	for(int i=0;i<N;i++){
	  //  printf("%lu\t",A[i]);
	}
        mergesort(A,0,N-1);
	//printf("\n");
	for(int i=0;i<N;i++){

//printf("%lu\t",A[i]);
        }

}
