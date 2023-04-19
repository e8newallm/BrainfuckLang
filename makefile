BINDIR = bin
OBJDIR = obj

SOURCES = $(wildcard parsetree/*.cpp)
HEADERS = $(wildcard parsetree/*.h)
OBJECTS = $(patsubst parsetree/%.cpp, $(OBJDIR)/parsetree/%.o, $(SOURCES))

all: parse


$(OBJDIR)/parse.tab.c $(OBJDIR)/parse.tab.h: metadata.h parse.y
	mkdir -p $(OBJDIR)
	bison -t -v -d parse.y -b $(OBJDIR)/parse

$(OBJDIR)/lex.yy.c: $(OBJDIR)/parse.tab.c $(OBJDIR)/parse.tab.h parse.l
	mkdir -p $(OBJDIR)
	flex -o $@ parse.l

$(OBJDIR)/parsetree/%.o: parsetree/%.cpp
	mkdir -p $(OBJDIR)/parsetree
	g++ -c $< -o $@

parse: $(OBJDIR)/lex.yy.c $(OBJDIR)/parse.tab.c $(OBJDIR)/parse.tab.h $(OBJECTS) $(HEADERS) process.cpp process.h metadata.h
	mkdir -p $(BINDIR)
	g++ -o $(BINDIR)/parse -I . $^

clean:
	rm -r $(BINDIR) $(OBJDIR)