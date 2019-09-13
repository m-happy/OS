%{
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#include "y.tab.h"
int c;
%}

%%
" "					{return SPACE;}
"\n"				{return NEWLINE;}
":"					{return COLON;}
[0-9]+				{yylval.a = yytext[0] - '0'; return NUMBER;}
"\t"				{return TAB;}
"="					{return EQUAL;}
"=="				{return CEQUAL;}
INIT				{return INIT;}
Select 				{return SELECT;}
Delay 				{return DELAY;}
if 					{return IF;}
Else				{return ELSE;}
readInt				{return READINT;}
int 				{return INT;}
[a-bA-z0-9_]+		{strcpy(yylval.s, yytext); return IDENTIFIER;}
^#(.)*\n 			{}
%%