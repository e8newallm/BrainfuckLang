#ifndef COUTCSTRING_H
#define COUTCSTRING_H

#include <string>

#include "parsetree.h"

class CoutCString : public ParseTree
{
    public:
    CoutCString(std::string cstring, ParseTree* node);
    virtual std::string process() override;

    private:
    std::string text;
};

#endif //COUTCSTRING_H