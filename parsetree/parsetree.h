#ifndef PARSETREE_H
#define PARSETREE_H

#include <string>
#include <vector>

class ParseTree
{
    public:
    template <typename... Args>
    ParseTree(Args... args)
    {
        childNode = {args...};
    };

    virtual std::string process()
    {
        std::string result = "";
        for(ParseTree* childNode : childNode)
        {
            result += childNode->process();
        }
        return result;
    };

    ~ParseTree()
    {
        for(ParseTree* childNode : childNode)
        {
            delete childNode;
        }
    }

    protected:
    std::vector<ParseTree*> childNode;
};

#include "print.h"
#include "assignment.h"

#endif //PARSETREE_H