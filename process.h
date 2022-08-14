#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

#include "metadata.h"

extern std::vector<varEntry<int>*> integerTable;
extern std::vector<varEntry<std::string>*> stringTable;

void processData();
std::string generateBrainfuck();

#endif //PROCESS_H