%{

#include <iostream>
#include <string>
#include <fstream>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

#include "metadata.h"
#include "process.h"
#include "parsetree/parsetree.h"
#include "highlevelfuncs.h"
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

%token T_PLUS T_MINUS T_ASTERISK T_BACKSLASH T_LPAREN T_RPAREN T_EQUALS T_PLUSEQUALS T_QUOTE T_OUTPUT
%token T_NEWLINE T_SEMICOLON

%token NUMBER STRING PRINT

%start program

%type<tree> program statements statement printfunc varAss varAdd printstatement

%%

program: {start = new ParseTree();} | statements {start = new ParseTree($1);};

statements:
      statement T_SEMICOLON statements {$$ = new ParseTree($1, $3);}
	| statement T_SEMICOLON {$$ = new ParseTree($1);};

statement: 
    varDec {$$ = new ParseTree();}
    | printfunc {$$ = new ParseTree($1);}
    | varAss {$$ = new ParseTree($1);};
    | varAdd {$$ = new ParseTree($1);};

printfunc: PRINT printstatement {$$ = new ParseTree($2);};

printstatement: {$$ = new ParseTree();}
    | T_OUTPUT T_CSTRING printstatement
    {
        std::string cstring = std::string($2->pointer, $2->size);
        bool constFound = false;
        for(VarEntry* entry : varTable)
        {
            if(entry->varName == cstring)
                constFound = true;
        }
        if(!constFound)
            varTable.push_back(new VarEntry(cstring, cstring));
        $$ = new Print(cstring, $3);
    }
    | T_OUTPUT T_TEXT printstatement
    {
        std::string varName = std::string($2->pointer, $2->size);
        bool varFound = false;
        for(VarEntry* entry : varTable)
        {
            if(entry->varName == varName)
            {
                $$ = new Print(varName, $3);
                varFound = true;
            }
        }
        if(!varFound)
        {
            yyerror("Var not found!");
        }
    }

varDec: 
    STRING T_TEXT 
    {
        varTable.push_back(new VarEntry(std::string($2->pointer, $2->size), std::string("")));
        delete((charSize*)$2);
    }

    | STRING T_TEXT T_EQUALS T_CSTRING
    {
        varTable.push_back(new VarEntry(std::string($2->pointer, $2->size), std::string($4->pointer, $4->size)));
        delete((charSize*)$2);
        delete((charSize*)$4);
    };

    | NUMBER T_TEXT
    {
        varTable.push_back(new VarEntry(std::string($2->pointer, $2->size), 0));
        delete((charSize*)$2);
    }

    | NUMBER T_TEXT T_EQUALS T_INT
    {
        varTable.push_back(new VarEntry(std::string($2->pointer, $2->size), $4));
        delete((charSize*)$2);
    };

varAss: 
    T_TEXT T_EQUALS T_TEXT
    {
        std::string varName = std::string($1->pointer, $1->size);
        VarEntry* firstVar;
        VarEntry* secondVar;
        bool varFound = false;
        for(VarEntry* entry : varTable)
        {
            if(entry->varName == varName)
            {
                varFound = true;
                firstVar = entry;
                break;
            }
        }
        if(!varFound)
        {
            yyerror("Var not found!");
        }

        varName = std::string($3->pointer, $3->size);
        varFound = false;
        for(VarEntry* entry : varTable)
        {
            if(entry->varName == varName)
            {
                varFound = true;
                secondVar = entry;
                break;
            }
        }
        if(!varFound)
        {
            yyerror("Var not found!");
        }
        $$ = new Assignment(firstVar, secondVar);
    }
    
    | T_TEXT T_EQUALS T_INT
    {
        std::string varName = std::string($1->pointer, $1->size);
        bool varFound = false;
        for(VarEntry* entry : varTable)
        {
            if(entry->varName == varName)
            {
                varFound = true;
                $$ = new Assignment(entry, $3);
                break;
            }
        }
        if(!varFound)
        {
            yyerror("Var not found!");
        }
        
    };

varAdd:
    T_TEXT T_PLUSEQUALS T_INT
    {
        std::string varName = std::string($1->pointer, $1->size);
        bool varFound = false;
        for(VarEntry* entry : varTable)
        {
            if(entry->varName == varName)
            {
                varFound = true;
                $$ = new Assignment(entry, $3, add);
                break;
            }
        }
        if(!varFound)
        {
            yyerror("Var not found!");
        }
        
    };
%%

int main()
{
	yyin = stdin;
	yyparse();
    std::string result = generateBrainfuck();
    
    std::cout << result;
	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}