#include "assignment.h"
#include "../highlevelfuncs.h"

Assignment::Assignment(VarEntry* varDest, VarEntry* varSource, SumType type) : ParseTree()
{
    this->varDest = varDest;
    this->varSource = varSource;
    this->type = type;
}

Assignment::Assignment(VarEntry* varDest, int valSource, SumType type) : ParseTree()
{
    this->varDest = varDest;
    this->valSource = valSource;
    this->type = type;
}

std::string Assignment::process()
{
    if(varSource)
    {
        return copyNum(varSource->memoryPosition, varDest->memoryPosition);
    }
    else
    {
        switch(type)
        {
            case equals:
                return zeroNum(varDest->memoryPosition) + addConstNum(varDest->memoryPosition, valSource);
            case add:
                return addConstNum(varDest->memoryPosition, valSource);
        }
    }
    return "";
}