#include <string>

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
    for(VarEntry<std::string>* entry : cstringTable)
    {
        if(entry->initialValue == text)
        {
            result = printMessage(entry->memoryPosition);
        }
    }
    return result + "\r\n" + childNode[0]->process();
};