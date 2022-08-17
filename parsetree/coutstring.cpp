#include <string>
#include <iostream>

#include "coutstring.h"
#include "../process.h"

CoutString::CoutString(std::string cstring, ParseTree* node) : ParseTree(node)
{
    text = cstring;
    childNode[0] = node;
}

std::string CoutString::process()
{
    std::string result = "ERROR";
    int pos = 0;
    for(VarEntry<std::string>* entry : stringTable)
    {
        if(entry->varName == text)
        {
            result = printMessage(entry->memoryPosition);
            return result + "\r\n" + childNode[0]->process();
        }
    }
    return "";
};