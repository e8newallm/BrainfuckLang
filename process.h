#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

#include "metadata.h"

extern std::vector<VarEntry<int>*> integerTable;
extern std::vector<VarEntry<std::string>*> stringTable;

void processData();
std::string generateBrainfuck();

#endif //PROCESS_H