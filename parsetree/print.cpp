#include <string>
#include <iostream>

#include "print.h"
#include "../process.h"
#include "../highlevelfuncs.h"

std::string printString(int position = 0)
{
    return movTmp("[.>]<[<]>", position);
}

Print::Print(std::string string, ParseTree* node) : ParseTree(node)
{
    text = string;
    childNode[0] = node;
}

std::string Print::process()
{
    std::string result = "ERROR";
    int pos = 0;
    for(VarEntry* entry : varTable)
    {
        if(entry->varName == text)
        {
            result = printString(entry->memoryPosition);
            return result + childNode[0]->process();
        }
    }
    return "";
};