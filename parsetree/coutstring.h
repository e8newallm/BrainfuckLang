#ifndef COUTSTRING_H
#define COUTSTRING_H

#include <string>

#include "parsetree.h"

class CoutString : public ParseTree
{
    public:
    CoutString(std::string cstring, ParseTree* node);
    virtual std::string process() override;

    private:
    std::string text;
};

#endif //COUTSTRING_H