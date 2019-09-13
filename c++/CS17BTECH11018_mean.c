#include <stdio.h>
int main(){
	int n;
	printf("Enter the no of elements in the array\n");
	scanf("%d", &n);
	int arr[n];
	float sum = 0;
	double mean;
	for(int i = 0; i < n; i++){
		printf("Enter the %d number : ",i + 1);
		scanf("%d",&arr[i]);
		sum += arr[i];
	}
	mean = sum/ n;
	printf("%lf\n",mean );

}