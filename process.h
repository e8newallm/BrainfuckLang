#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

#include "metadata.h"
#include "parsetree/parsetree.h"

extern std::vector<VarEntry*> varTable;

extern ParseTree* start;

std::string generateBrainfuck();
std::string optimise(std::string finalCode);

#endif //PROCESS_H