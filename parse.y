%{

#include <iostream>
#include <string>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

#include "metadata.h"
#include "process.h"

%}

%code requires {
    #include <iostream>

    struct charSize {
        char* pointer; 
        int size;
        charSize(char* pointerIn, int sizeIn) {pointer = pointerIn; size = sizeIn;};
    };
}

%union {
	int ival;
	float fval;
    charSize* sval;
}

%token<ival> T_INT
%token<fval> T_FLOAT
%token<sval> T_TEXT T_CSTRING

%token T_PLUS T_MINUS T_ASTERISK T_BACKSLASH T_LPAREN T_RPAREN T_EQUALS T_QUOTE
%token T_NEWLINE T_SEMICOLON

%token INT STDSTRING

%start program

%%

program: statement T_SEMICOLON program
	   | statement T_SEMICOLON;

statement: varDec;

varDec: intDec | strDec;

strDec: STDSTRING T_TEXT 
        {
            stringTable.push_back(new VarEntry<std::string>(std::string($2->pointer, $2->size), std::string("TEST")));
            delete((charSize*)$2);
        }

      | STDSTRING T_TEXT T_EQUALS T_CSTRING
        {
            stringTable.push_back(new VarEntry<std::string>(std::string($2->pointer, $2->size), std::string($4->pointer, $4->size)));
            delete((charSize*)$2);
            delete((charSize*)$4);
        };


intDec: INT T_TEXT 
        {integerTable.push_back(new VarEntry<int>(std::string($2->pointer, $2->size), 0)); delete((charSize*)$2);}

      | INT T_TEXT T_EQUALS T_INT 
        {integerTable.push_back(new VarEntry<int>(std::string($2->pointer, $2->size), $4)); delete((charSize*)$2);};

%%

int main() {
	yyin = stdin;
	yyparse();
    processData();
    std::string result = generateBrainfuck();

    std::cout << "THE RESULT:\r\n";
    std::cout << result;
	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}