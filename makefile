BINDIR = bin
OBJDIR = obj

SOURCES = $(wildcard parsetree/*.cpp)
HEADERS = $(wildcard parsetree/*.h)
OBJECTS = $(patsubst parsetree/%.cpp, $(OBJDIR)/parsetree/%.o, $(SOURCES))

all: $(BINDIR)/parse


$(OBJDIR)/parse.tab.o: metadata.h parse.y highlevelfuncs.h
	mkdir -p $(OBJDIR)
	bison -t -v -d parse.y -b $(OBJDIR)/parse
	g++ -I . -c $(OBJDIR)/parse.tab.c -o $@

$(OBJDIR)/lex.yy.c: $(OBJDIR)/parse.tab.o parse.l
	mkdir -p $(OBJDIR)
	flex -o $@ parse.l

$(OBJDIR)/parsetree/%.o: parsetree/%.cpp metadata.h highlevelfuncs.h
	mkdir -p $(OBJDIR)/parsetree
	g++ -c $< -o $@

$(OBJDIR)/process.o: process.cpp process.h metadata.h highlevelfuncs.h
	mkdir -p $(OBJDIR)
	g++ -c $< -o $@

$(BINDIR)/parse: $(OBJDIR)/lex.yy.c $(OBJDIR)/parse.tab.o $(OBJDIR)/process.o $(OBJECTS) $(HEADERS)
	mkdir -p $(BINDIR)
	g++ -o $@ -I . $^

clean:
	rm -r $(BINDIR) $(OBJDIR)