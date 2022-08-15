#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

#include "metadata.h"
#include "pointermovement.h"
#include "parsetree/parsetree.h"

extern std::vector<VarEntry<int>*> integerTable;
extern std::vector<VarEntry<std::string>*> stringTable;
extern std::vector<VarEntry<std::string>*> cstringTable;

extern PointerMovement pointerPos;
extern ParseTree* start;

void processData();
std::string generateBrainfuck();
std::string printMessage(int position);
#endif //PROCESS_H