#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

#include "metadata.h"
#include "parsetree/parsetree.h"

extern int maxSumLevels;
extern int currentSumLevels;

extern std::vector<VarEntry<int>*> integerTable;
extern std::vector<VarEntry<std::string>*> stringTable;
extern std::vector<VarEntry<std::string>*> cstringTable;

extern std::vector<std::string> varNamesTable;

extern ParseTree* start;

void processData();
std::string generateBrainfuck();
std::string beautify(std::string finalCode);
std::string printMessage(int position);
#endif //PROCESS_H