#include <iostream>
#include <string>
using namespace std;
int main(){
	string str;
	getline(cin, str);
	//displaying the string
	cout<<str<<endl;
	//push_back is used for inserting character in the end
	str.push_back('s');
	//pop_back is used for deleting the last character of the strning
	str.pop_back();
	//capacity function is the capacity of function and it is dynamic
	//and can be expanded at any point of time
	cout<<str.capacity()<<endl;
	//resize is used for the resize the string with the required size as you want
	// str.resize(1);
	//it is for shrinking the function to fit the string size
	str.shrink_to_fit();
	//finding the length of the string
	cout<<str.length()<<endl;
	cout<<str.size()<<endl;

	// cout<<str.capacity()<<endl;
	cout<<str<<endl;
	//declaring iterator
	string::iterator it;
	//declaring reverse iterator
	string::reverse_iterator it1;
	for(it = str.begin(); it!= str.end(); it++)
		cout<<*it;
	cout<<endl;
	for(it1 = str.rbegin(); it1!= str.rend(); it1++)
		cout<<*it;
	cout<<endl;
	//copy function in string
	char ch[15];
	str.copy(ch, 14, 0);
	cout<<ch<<endl;
	string str1 = "h";
	//swap function is used for swaping the two arrays
	str.swap(str1);
	//compare() function returns the difference of two strings in integer
	cout<<str.compare(str1);
	//find() function searches for the first occurence of the substring in the string
	// cout<<str.find("james");
	//rfind() function return the last occurence of the string
	// cout<<str.rfind("james");
	//insert() functions insert the given substring in the string
	str1.insert(5, "arrays");
	//clear() functions clears the strings
	str.clear();
	//empty() function checks if the string is empty or not
	str.empty();	

	cout<<str<<endl;
	cout<<str1<<endl;
	return 0;
}