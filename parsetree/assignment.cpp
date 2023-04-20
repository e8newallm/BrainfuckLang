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
        switch(type)
        {
            case equals:
                return copyCell(varSource->memoryPosition, varDest->memoryPosition);
            case add:
                return addCell(varSource->memoryPosition, varDest->memoryPosition);
            case sub:
                return subCell(varSource->memoryPosition, varDest->memoryPosition);
            case multiply:
                return mulCell(varSource->memoryPosition, varDest->memoryPosition);
        }
    }
    else
    {
        switch(type)
        {
            case equals:
                return zeroCell(varDest->memoryPosition) + addConst(valSource, varDest->memoryPosition);
            case add:
                return addConst(valSource, varDest->memoryPosition);
            case sub:
                return subConst(valSource, varDest->memoryPosition);
            case multiply:
                return mulConst(valSource, varDest->memoryPosition);
        }
    }
    return "";
}