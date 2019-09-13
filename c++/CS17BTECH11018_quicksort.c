#include <stdio.h>
//function to swap the values of the array
int swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}
//function for creating the partion of the array around the pivot
int partition(int arr[], int l, int r){
	int pivot = arr[r];
	int i = l -1;
	for(int j = l; j <= r -1 ; j++){
		if(arr[j] <= pivot){
			i++;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i+1], &arr[r]);
	return i+ 1;
}
// function for sorting the array
void quickSort(int arr[], int l, int r){
	if(l < r){
		int pivot = partition(arr, l, r);
		quickSort(arr, l, pivot-1);
		quickSort(arr, pivot + 1, r);
	}
}
//main function
int main(){
	int n;
	printf("Enter the no of elements in the array : ");
	scanf("%d", &n);
	int arr[n];
	for(int i = 0; i < n ; i++)
		scanf("%d", &arr[i]);
	printf("\nGiven array is : ");
	for (int i = 0; i < n; ++i)
	{
		printf("%d ",arr[i] );
	}
	quickSort(arr, 0, n -1 );	//function called
	printf("\nArray after sorting : ");
	for (int i = 0; i < n; ++i)
	{
		printf("%d ",arr[i] );
	}
	return 0;
}