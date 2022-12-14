#include <string>
#include <iostream>

#include "coutcstring.h"
#include "../process.h"

CoutCString::CoutCString(std::string cstring, ParseTree* node) : ParseTree(node)
{
    text = cstring;
    childNode[0] = node;
}

std::string CoutCString::process()
{
    std::string result = "ERROR";
    int pos = 0;
    for(VarEntry<std::string>* entry : cstringTable)
    {
        if(entry->varName == text)
        {
            result = printMessage(entry->memoryPosition);
            return result + "\r\n" + childNode[0]->process();
        }
    }
    return "";
};