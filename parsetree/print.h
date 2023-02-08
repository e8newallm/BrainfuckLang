#ifndef PRINT_H
#define PRINT_H

#include <string>

#include "parsetree.h"

class Print : public ParseTree
{
    public:
    Print(std::string string, ParseTree* node);
    Print(int number, ParseTree* node);
    virtual std::string process() override;

    private:
    std::string text;
    int number;
};

#endif //PRINT_H