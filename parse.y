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

%token T_PLUS T_MINUS T_ASTERISK T_BACKSLASH T_LPAREN T_RPAREN T_EQUALS T_PLUSEQUALS T_MINUSEQUALS T_MULTIEQUALS T_DIVIDEEQUALS T_QUOTE T_OUTPUT
%token T_NEWLINE T_SEMICOLON

%token NUMBER STRING PRINT

%start program

%type<tree> program statements statement printfunc varDec varAss varAdd varSub varMul varDiv printstatement

%%

program: {start = new ParseTree();} | statements {start = new ParseTree($1);};

statements:
      statement T_SEMICOLON statements {$$ = new ParseTree($1, $3);}
	| statement T_SEMICOLON {$$ = new ParseTree($1);};

statement: 
    varDec {$$ = new ParseTree($1);}
    | printfunc {$$ = new ParseTree($1);}
    | varAss {$$ = new ParseTree($1);};
    | varAdd {$$ = new ParseTree($1);};
    | varSub {$$ = new ParseTree($1);};
    | varMul {$$ = new ParseTree($1);};
    | varDiv {$$ = new ParseTree($1);};

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
    };

varDec: 
    STRING T_TEXT 
    {
        varTable.push_back(new VarEntry(std::string($2->pointer, $2->size), std::string("")));
        $$ = new ParseTree();
        delete((charSize*)$2);
    }

    | STRING T_TEXT T_EQUALS T_CSTRING
    {
        varTable.push_back(new VarEntry(std::string($2->pointer, $2->size), std::string($4->pointer, $4->size)));
        $$ = new ParseTree();
        delete((charSize*)$2);
        delete((charSize*)$4);
    };

    | NUMBER T_TEXT
    {
        varTable.push_back(new VarEntry(std::string($2->pointer, $2->size), 0));
        $$ = new ParseTree();
        delete((charSize*)$2);
    }

    | NUMBER T_TEXT T_EQUALS T_INT
    {
        varTable.push_back(new VarEntry(std::string($2->pointer, $2->size), $4));
        $$ = new ParseTree();
        delete((charSize*)$2);
    };
    
    | NUMBER T_TEXT T_EQUALS T_TEXT
    {
        VarEntry* firstVar = new VarEntry(std::string($2->pointer, $2->size), 0);
        varTable.push_back(firstVar);

        VarEntry* secondVar;
        std::string varName = std::string($4->pointer, $4->size);
        bool varFound = false;
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
        
    }
    | T_TEXT T_PLUSEQUALS T_TEXT
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
        $$ = new Assignment(firstVar, secondVar, add);
    };

varSub:
    T_TEXT T_MINUSEQUALS T_INT
    {
        std::string varName = std::string($1->pointer, $1->size);
        bool varFound = false;
        for(VarEntry* entry : varTable)
        {
            if(entry->varName == varName)
            {
                varFound = true;
                $$ = new Assignment(entry, $3, sub);
                break;
            }
        }
        if(!varFound)
        {
            yyerror("Var not found!");
        }
        
    }
    | T_TEXT T_MINUSEQUALS T_TEXT
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
        $$ = new Assignment(firstVar, secondVar, sub);
    };

varMul:
    T_TEXT T_MULTIEQUALS T_INT
    {
        std::string varName = std::string($1->pointer, $1->size);
        bool varFound = false;
        for(VarEntry* entry : varTable)
        {
            if(entry->varName == varName)
            {
                varFound = true;
                $$ = new Assignment(entry, $3, multiply);
                break;
            }
        }
        if(!varFound)
        {
            yyerror("Var not found!");
        }
        
    }
    | T_TEXT T_MULTIEQUALS T_TEXT
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
        $$ = new Assignment(firstVar, secondVar, multiply);
    };

varDiv:
    T_TEXT T_DIVIDEEQUALS T_INT
    {
        std::string varName = std::string($1->pointer, $1->size);
        bool varFound = false;
        for(VarEntry* entry : varTable)
        {
            if(entry->varName == varName)
            {
                varFound = true;
                $$ = new Assignment(entry, $3, divide);
                break;
            }
        }
        if(!varFound)
        {
            yyerror("Var not found!");
        }
        
    }
    | T_TEXT T_DIVIDEEQUALS T_TEXT
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
        $$ = new Assignment(firstVar, secondVar, divide);
    };
%%

int main()
{
	yyin = stdin;
	yyparse();
    std::string result = generateBrainfuck();
    const int width = 240;
    for(int i = width-1; i < result.size(); i += width)
    {
        result.insert(i, "\n");
    }
    std::cout << result << "\r\n";
	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}