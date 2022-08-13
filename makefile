all: parse

parse.tab.c parse.tab.h:	parse.y
	bison -t -v -d parse.y

lex.yy.c: parse.l parse.tab.h
	flex parse.l

parse: lex.yy.c parse.tab.c parse.tab.h
	gcc -o parse parse.tab.c lex.yy.c

clean:
	rm parse parse.tab.c lex.yy.c parse.tab.h parse.output