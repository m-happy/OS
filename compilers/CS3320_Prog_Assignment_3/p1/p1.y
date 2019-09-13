%{
	#include <stdio.h>
	#include <iostream>
	using namespace std;
	#include "y.tab.h"
	int yylex(void);
	void yyerror(const char *);


	char board[8][8] = {
		{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
		{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
		{'R', 'N', 'B', 'K', 'Q', 'B', 'N', 'R'}
	};	

	void configuration(){
		for(int i = 0; i < 8; i++){
			for(int j = 0; j < 8; j++){
				cout<<board[i][j]<<"\t";
			}
			cout<<endl;
		}
	}
	void make_move (int type, int c1, int r1, int c2, int r2){
		//cout<<"hello"<<endl;
		//cout<<r1<<c1<<r2<<c2<<endl;
		if(type == 0){
			//cout<<"pawn"<<endl;
			char h = board[r1-1][c1];
			//cout<<h<<endl;
			board[r1-1][c1] = ' ';
			board[r2-1][c2] = h;
		}
		else{
			board[r1-1][c1] = ' ';
			board[r2-1][c2] = type;
		}
	}
%}
%start move
%union { int a;}
%type<a> HMOVE ROW COLUMN
%token  HMOVE SPACE PAWNMOVE CAPTURE MOVE ROW COLUMN NEWLINE

%%
move : turn SPACE turn NEWLINE move |
		turn SPACE turn { configuration();}|
		turn SPACE turn NEWLINE {configuration();};
turn : ROW COLUMN MOVE ROW COLUMN 	{ make_move(0, $1, $2, $4, $5);}|
		ROW COLUMN CAPTURE ROW COLUMN {make_move(0, $1, $2, $4, $5);}|
		HMOVE ROW COLUMN MOVE ROW COLUMN { make_move($1, $2, $3, $5, $6);}|
		HMOVE ROW COLUMN CAPTURE ROW COLUMN {make_move($1, $2, $3, $5, $6);};

%%
int yywrap(void){
	return 1;
}
void yyerror( char const *s){
	fprintf(stderr, "%s\n",s);
}
int main(void) 
{
	return (yyparse());
}