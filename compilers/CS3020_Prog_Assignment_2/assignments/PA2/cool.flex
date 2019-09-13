/* 
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
*/
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */
 int comments = 0;

%}

/*
 * Define names for regular expressions here.
 */
whitespace [ \f\r\t\v]

ints    [0-9]+
types   [A-Z][a-zA-Z0-9_]*
objects   [a-z][a-zA-Z0-9_]*

SINGLES   "+"|"-"|"*"|"/"|"~"|"<"|"="|"("|")"|"{"|"}"|";"|":"|"."|","|"@"
INVALID   "`"|"!"|"#"|"$"|"%"|"^"|"&"|"_"|"["|"]"|"|"|[\\]|">"|"?"

%x comment string escape 

DARROW          =>
ASSIGN          <-
LE              <=

%%

{DARROW}		{ return (DARROW); }
{ASSIGN}    {return (ASSIGN);}
{LE}        { return (LE);}


 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  */
(?i:class)    return CLASS;
(?i:not) return NOT;
(?i:fi)     return FI;
(?i:if)     return IF;
(?i:in)     return IN;
(?i:else)   return ELSE;
(?i:isvoid)   return ISVOID;
(?i:inherits)   return INHERITS;
(?i:loop)   return LOOP;
(?i:pool)   return POOL;
(?i:then)   return THEN;
(?i:while)    return WHILE;
(?i:let)    return LET;
(?i:case)   return CASE;
(?i:esac)   return ESAC;
(?i:of)     return OF;
(?i:new)    return NEW;

"false"   {
        cool_yylval.boolean = false;
        return BOOL_CONST;
}
"true"   {
        cool_yylval.boolean = true;
        return BOOL_CONST;
      }


 /*
  * Identifiers for Ints, Types and Objects
  */
{ints}      {
        cool_yylval.symbol = inttable.add_string(yytext);
        return INT_CONST;
      }
{types}     {
        cool_yylval.symbol = idtable.add_string(yytext);
        return TYPEID;
      }
{objects}|(self)  {
        cool_yylval.symbol = idtable.add_string(yytext);
        return OBJECTID;
      }

 /*
  * Single Character Special Syntactic Symbols
  */
{SINGLES}   return int(yytext[0]);

 /*
  * Single Invalid Characters
  */
{INVALID}   {
        cool_yylval.error_msg = yytext;
        return ERROR;
}
 /*
  *  Nested comments
  */

"--"(.)* 

"*)"      {
        cool_yylval.error_msg = "unmatched *)";
        return ERROR;
      }
"(*"      {
        ++comments;
        BEGIN(comment);       
      }

<comment>"(*"   ++comments;
<comment>"*)"   {

        --comments;
        if(comments == 0)
            BEGIN(INITIAL);
        else if(comments < 0){
            cool_yylval.error_msg = "unmatched *)";
            comments = 0;
            BEGIN(INITIAL);
            return ERROR;
        }
      }
<comment>\n   ++curr_lineno;
<comment>.
<comment>{whitespace}+
<comment><<EOF>>  {
        BEGIN(INITIAL);
        if(comments > 0){
          cool_yylval.error_msg = "EOF in comment.";
          comments = 0;
          return ERROR;
        }
      }


 /*
  *  String constants (C syntax)
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  *
  */
"\""      {
        BEGIN(string);
        string_buf_ptr = string_buf;
      }

<string>"\""    {
        if(string_buf_ptr - string_buf > MAX_STR_CONST-1){
          *string_buf = '\0';
          cool_yylval.error_msg = "string constant too long";
          BEGIN(escape);
          return ERROR;
        }
        *string_buf_ptr = '\0';
        cool_yylval.symbol = stringtable.add_string(string_buf);
        BEGIN(INITIAL);
        return STR_CONST;
      }
<string><<EOF>>   {
        cool_yylval.error_msg = "EOF in string constant";
        BEGIN(INITIAL);
        return ERROR;
      }
<string>\0    {
        *string_buf = '\0';
        cool_yylval.error_msg = "string contains null character";
        BEGIN(escape);
        return ERROR;
      }
<string>\n    {
        *string_buf = '\0';
        BEGIN(INITIAL);
        cool_yylval.error_msg = "unterminated string constant";
        return ERROR;
      }
<string>"\\n"   *string_buf_ptr++ = '\n';
<string>"\\b"   *string_buf_ptr++ = '\b';
<string>"\\t"   *string_buf_ptr++ = '\t';
<string>"\\f"   *string_buf_ptr++ = '\f';
<string>"\\"[^\0] *string_buf_ptr++ = yytext[1];
<string>.   *string_buf_ptr++ = *yytext;

<escape>[\n|"]    BEGIN(INITIAL);
<escape>[^\n|"] 


\n    curr_lineno++;
{whitespace}+

.   {
      cool_yylval.error_msg = yytext;
      return ERROR;
}

%%
