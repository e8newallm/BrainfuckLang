#ifndef METADATA_H
#define METADATA_H

#include <vector>
#include <ostream>

template<typename T> class VarEntry
{
    public:
    std::string varName;
    T initialValue;
    int memoryPosition = 0;
    VarEntry(std::string varNameIn, T initialValueIn)
    {
        varName = varNameIn;
        initialValue = initialValueIn;
    }

    void setPosition(int position)
    {
        memoryPosition = position;
    }

    int getPosition()
    {
        return memoryPosition;
    }
};

template<class T> inline std::ostream& operator<<(std::ostream& out, const VarEntry<T>& val){
    out << "variable name (" << val.varName << ") - initial value (" << val.initialValue << ") - assigned position (" << val.memoryPosition << ")";
    return out;
}

#endif //METADATA_H