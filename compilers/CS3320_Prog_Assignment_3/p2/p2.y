%{
	#include <stdio.h>
	#include <iostream>
	#include <string>
	using namespace std;
	#include "y.tab.h"
	int yylex(void);
	void yyerror(const char *);
	FILE* file = fopen("out.c","w");

%}
%union {int a; char s[100];}
%type<a> SELECT COLON NUMBER TAB
%type<s> IDENTIFIER
%token INIT SELECT DELAY NEWLINE NUMBER SPACE COLON TAB READINT INT IF ELSE IDENTIFIER CEQUAL EQUAL
%%
stmt: select stmt| init stmt | delay stmt | display stmt | declaration stmt| assign stmt | stmt1 stmt | {fprintf(file,"\t}\n}");};
select : SELECT COLON SPACE NUMBER NEWLINE {fprintf(file, "\tselect(%d);\n", $4);};
delay : DELAY COLON SPACE NUMBER NEWLINE {fprintf(file, "\tdelay(%d);\n",$4);};
display : TAB NUMBER TAB NUMBER TAB NUMBER TAB NUMBER TAB NUMBER TAB NUMBER TAB NUMBER TAB NUMBER NEWLINE  {fprintf(file, "\twrite(strtol(\"%d%d%d%d%d%d%d%d\"));\n", $2, $4, $6, $8, $10, $12, $14, $16);};
declaration : IDENTIFIER COLON SPACE INT NEWLINE {fprintf(file, "\tint input;\n");};
assign : 	IDENTIFIER SPACE EQUAL SPACE READINT NEWLINE	{fprintf(file, "\t%s = readInt();\n", $1);};
init : INIT NEWLINE {fprintf(file, "\tinit();\n\twhile(1){\n\t");};
stmt1 : IF SPACE IDENTIFIER SPACE CEQUAL SPACE NUMBER COLON NEWLINE {fprintf(file, "\tif(%s == 0){\n\t", $3);} display stmt1 display ELSE COLON NEWLINE {fprintf(file, "\telse {\n\t\t");} stmt1 | IF SPACE IDENTIFIER SPACE CEQUAL SPACE NUMBER COLON NEWLINE {fprintf(file, "\tif(%s == 0){\n\t", $3);}  display stmt1 | display {fprintf(file, "}\n");};
%%
int yywrap(void){
	fclose(file);
	return 1;
}
void yyerror( char const *s){
	fprintf(stderr, "%s\n",s);
}
int main(void) 
{
	
	fprintf(file,"#include<stdio.h>\n#include<stdlib.h>\n#include<seven_segment.h>\nint main()\n{ \n");
	return (yyparse());
}