#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "parsetree.h"
#include "../metadata.h"

#include <string>

enum SumType
{
    equals = 0,
    add
};

class Assignment : public ParseTree
{
    public:
        Assignment(VarEntry* varDest, VarEntry* varSource, SumType type = equals);
        Assignment(VarEntry* varDest, int valSource, SumType type = equals);

        virtual std::string process() override;

    private:
        VarEntry* varDest;
        VarEntry* varSource;
        int valSource;
        SumType type;
};

#endif //ASSIGNMENT_H