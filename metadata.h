#ifndef METADATA_H
#define METADATA_H

#include <vector>
#include <ostream>

enum valueType
{
    number,
    string
};

class VarEntry
{
    public:
        std::string varName;

        std::string initialString;
        int initialNumber;
        valueType valType;

        int memoryPosition = 0;
        
        VarEntry(std::string varNameIn, std::string initialValueIn)
        {
            varName = varNameIn;
            initialString = initialValueIn;
            valType = string;
        }

        VarEntry(std::string varNameIn, int initialValueIn)
        {
            varName = varNameIn;
            initialNumber = initialValueIn;
            valType = number;
        }
};

#endif //METADATA_H