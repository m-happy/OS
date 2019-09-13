#include <stdio.h>
//function to swap the elements of array
void swap(int * a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}
//function to seclection sort
void selectionSort(int arr[], int n){
	int k;
	for(int i = 0; i < n -1; i++){
		k = i;
		for(int j = i + 1; j < n; j++){
			if(arr[j] > arr[k]){
				k = j;
			}
			swap(&arr[k], &arr[j]);
		}
	}
}
//driver function
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
	selectionSort(arr, n);
	printf("\nArray after sorting : ");
	for (int i = 0; i < n; ++i)
	{
		printf("%d ",arr[i] );
	}


}