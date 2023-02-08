#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "parsetree.h"
#include "../metadata.h"

#include <string>

class Assignment : public ParseTree
{
    public:
    Assignment(ParseTree* node, VarEntry* location);

    virtual std::string process() override;

    private:
    VarEntry* varLocation;
};

#endif //ASSIGNMENT_H