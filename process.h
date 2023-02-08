#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

#include "metadata.h"
#include "parsetree/parsetree.h"

extern int maxSumLevels;
extern int currentSumLevels;

extern std::vector<VarEntry*> varTable;

extern ParseTree* start;

void processData();
std::string generateBrainfuck();
std::string beautify(std::string finalCode);

#define movRight(delta) std::string(delta, '>')
#define movLeft(delta) std::string(delta, '<')
#define movTmp(scope, delta) movRight(delta) + scope + movLeft(delta)
#define inc(amount) std::string(amount, '+')
#define dec(amount) std::string(amount, '-')

#endif //PROCESS_H