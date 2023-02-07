%{

#include <iostream>
#include <string>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

#include "metadata.h"
#include "process.h"
#include "parsetree/parsetree.h"
%}

%code requires {
    #include <iostream>

    #include "parsetree/parsetree.h"

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
    ParseTree* tree;
}

%token<ival> T_INT
%token<fval> T_FLOAT
%token<sval> T_TEXT T_CSTRING

%token T_PLUS T_MINUS T_ASTERISK T_BACKSLASH T_LPAREN T_RPAREN T_EQUALS T_QUOTE T_OUTPUT
%token T_NEWLINE T_SEMICOLON

%token INT STRING PRINT

%start program

%type<tree> program statements statement cout printstatement

%%

program: {start = new ParseTree();} | statements {start = new ParseTree($1);};

statements:
      statement T_SEMICOLON statements {$$ = new ParseTree($1, $3);}
	| statement T_SEMICOLON {$$ = new ParseTree($1);};

statement: 
    varDec {$$ = new ParseTree();}
    | cout {$$ = new ParseTree($1);};

cout: PRINT printstatement {$$ = new ParseTree($2);};

printstatement: {$$ = new ParseTree();}
    | T_OUTPUT T_CSTRING printstatement
    {
        std::string cstring = std::string($2->pointer, $2->size);
        bool constFound = false;
        for(VarEntry<std::string>* entry : cstringTable)
        {
            if(entry->varName == cstring)
                constFound = true;
        }
        if(!constFound)
            cstringTable.push_back(new VarEntry<std::string>(cstring, cstring));
        $$ = new CoutCString(cstring, $3);
    }
    | T_OUTPUT T_TEXT printstatement
    {
        std::string varName = std::string($2->pointer, $2->size);
        bool varFound = false;
        for(VarEntry<std::string>* entry : stringTable)
        {
            if(entry->varName == varName)
            {
                $$ = new CoutString(varName, $3);
                varFound = true;
            }
        }
        if(!varFound)
        {
            yyerror("Var not found!");
        }
    }

varDec: strDec;

strDec: 
    STRING T_TEXT 
    {
        stringTable.push_back(new VarEntry<std::string>(std::string($2->pointer, $2->size), std::string("")));
        varNamesTable.push_back(std::string($2->pointer, $2->size));
        delete((charSize*)$2);
    }

    | STRING T_TEXT T_EQUALS T_CSTRING
    {
        stringTable.push_back(new VarEntry<std::string>(std::string($2->pointer, $2->size), std::string($4->pointer, $4->size)));
        varNamesTable.push_back(std::string($2->pointer, $2->size));
        delete((charSize*)$2);
        delete((charSize*)$4);
    };

%%

int main() {
	yyin = stdin;
	yyparse();
    processData();
    std::string result = generateBrainfuck();

    std::cout << result;
	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}