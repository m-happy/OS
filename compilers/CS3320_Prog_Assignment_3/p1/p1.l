%{
#include <stdio.h>
#include <iostream>
using namespace std;
#include "y.tab.h"
int c;
%}

%%
" " 									{return (SPACE);}
"-"										{return (MOVE);}
"x"										{return (CAPTURE);}	
[a-h]									{	c = yytext[0];
											yylval.a = c - 'a';
											return (ROW);	}
[1-8]									{
												c = yytext[0];
												yylval.a = c - '0';
												return (COLUMN);}
[KBNQR] 								{
											c = yytext[0];
											yylval.a = c ;
											return (HMOVE);}
"\n"									{return NEWLINE;}			
%%
