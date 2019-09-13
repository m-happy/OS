#include <iostream>
using namespace std;
//function for merge
void merge(int arr[], int l, int m, int r){
	int n1 = m - l + 1;
	int n2 = r - m;
	int L[n1], R[n2];
	for(int i = 0 ; i< n1; i++)
		L[i] = arr[l + i];
	for(int j = 0; j < n2; j++)
		R[j] = arr[m + j + 1];
	int i = 0; 
	int j = 0;
	int k = l;
	while(i < n1 && j < n2){
		if(L[i] <= R[j]){
			arr[k] = L[i]; i++;
		}
		else{
			arr[k] = R[j]; j++;
		}
		k++;
	}
	while(i < n1){
		arr[k] = L[i];
		k++; i++;
	}
	while(j < n2){
		arr[k] = R[j];
		j++; k++;
	}

}
//function for merge sort
void mergeSort(int arr[], int l, int r){
	int m;
	if(l < r){
		m = l + (r-l)/2;
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);
		merge(arr, l, m, r);
	}
}
int main(){
	int n;
	cout<<"Enter the no of elements in the array : ";
	cin>>n;
	int arr[n];
	for(int i = 0; i < n ; i++)
		scanf("%d", &arr[i]);
	printf("\nGiven array is : ");
	for (int i = 0; i < n; ++i)
	{
		printf("%d ",arr[i] );
	}
	mergeSort(arr, 0, n - 1);
	printf("\nArray after sorting : ");
	for (int i = 0; i < n; ++i)
	{
		printf("%d ",arr[i] );
	}
	return 0;

}