#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
int main(){
	// int size;
	// printf("Enter of the size of the string : ");
	// scanf("%d\n", &size);
	printf("Enter string : ");
	char s[1000];
	scanf("%[^\n]%*c", s);
	// printf("%s\n", s);
	int i = 0;
	int j = strlen(s);
	for(int z = 0 ; z < j; z++)
		s[z] = tolower(s[z]);
	// printf("%d\n", j);
	j = j -1;
	_Bool flag = true;
	while(i <= j){
		if(s[i] == s[j]){
			i++;
			j--;
		}
		else{
			printf("%s\n","String is not palindrome");
			flag = false;
			break;
		}
	}
	if(flag == true)	
		printf("%s\n","string is palindrome");
	return 0;
}