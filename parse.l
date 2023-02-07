%option noyywrap

%{
#include <stdio.h>
#include <cstring>

#define YY_DECL int yylex()

#include "parse.tab.h"
%}

%%

"//".*                      {std::cout << "Comment: " << yytext << "\r\n";} //comment
[ \t\r\n]	                ; // ignore all whitespace
[0-9]+\.[0-9]+ 	            {yylval.fval = atof(yytext); return T_FLOAT;}
[0-9]+	    	            {yylval.ival = atoi(yytext); return T_INT;}
"+"		                    {return T_PLUS;}
"-"		                    {return T_MINUS;}
"*"		                    {return T_ASTERISK;}
"/"		                    {return T_BACKSLASH;}
"("		                    {return T_LPAREN;}
")"		                    {return T_RPAREN;}
";"                         {return T_SEMICOLON;}
"="                         {return T_EQUALS;}
"<<"                        {return T_OUTPUT;}

"int"                       {return INT;}
"string"                    {return STRING;}
"print"                     {return PRINT;}

["]([^"]|\\\")*([^\\]["])   {yylval.sval = new charSize(yytext+1, yyleng-2); return T_CSTRING;}
[A-Za-z]+                   {yylval.sval = new charSize(yytext, yyleng); return T_TEXT;}
%%