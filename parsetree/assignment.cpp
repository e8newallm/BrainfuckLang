#include "assignment.h"

Assignment::Assignment(ParseTree* node, VarEntry* location) : ParseTree(node)
{
    varLocation = location;
}

std::string Assignment::process()
{
    
}