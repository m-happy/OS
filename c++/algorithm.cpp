#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>
using namespace std;
int main(){
	int arr[] = {10, 20, 4, 2 , 12};
	int n = sizeof(arr)/sizeof(arr[0]);
	vector <int> vect(arr, arr+n);
	cout<<"Vector is: ";
	for(int i = 0; i < n;  i++)
		cout<<vect[i]<<" ";
	//sorting the vector in ascending order
	sort(vect.begin(), vect.end());
	cout<<endl;
	cout<<"vector after sorting";
	for(int i = 0; i < n;  i++)
		cout<<vect[i]<<" ";
	cout<<endl;
	//reversing the vector
	reverse(vect.begin(), vect.end());
	cout<<"vector after reversing";
	for(int i = 0; i < n;  i++)
		cout<<vect[i]<<" ";
	//for max element of a vector
	cout<<endl;
	cout<<"max element";
	cout<<*max_element(vect.begin(), vect.end());
	cout<<endl;
	cout<<"min element";
	cout<<*min_element(vect.begin(), vect.end());
	cout<<endl;
	//for calculating the sum of the vector
	cout<<accumulate(vect.begin(), vect.end(), 0);
	//for counting the occurense of an element in a vector
	cout<<"no of occurense of 20 ";
	cout<<count(vect.begin(), vect.end(), 20);
	cout<<endl;
	//find() operation find the element in a vector
	find(vect.begin(), vect.end(), 5) != vect.end() ? cout<<"Element found": cout<<"\nelement not found";
	auto q = lower_bound(vect.begin(), vect.end(), 20);
	auto p = upper_bound(vect.begin(), vect.end(), 20);






}