#include <iostream>
#include <string>

#include "metadata.h"
#include "pointermovement.h"
#include "parsetree/parsetree.h"

std::vector<VarEntry<int>*> integerTable;
std::vector<VarEntry<std::string>*> stringTable;
std::vector<VarEntry<std::string>*> cstringTable;

PointerMovement pointerPos;
ParseTree* start;

void processData()
{
    int positionAssigner = 0;

    for(VarEntry<std::string>* entry : cstringTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        std::cout << "string size: " << entry->initialValue.size() << "\r\n";
        entry->memoryPosition = positionAssigner;
        positionAssigner += entry->initialValue.size() + 2;
        positionAssigner++;
    }

    for(VarEntry<int>* entry : integerTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        entry->memoryPosition = positionAssigner;
        positionAssigner++;
    }

    for(VarEntry<std::string>* entry : stringTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        std::cout << "string size: " << entry->initialValue.size() << "\r\n";
        entry->memoryPosition = positionAssigner;
        positionAssigner += entry->initialValue.size() + 2;
        positionAssigner++;
    }
}

std::string printMessage(int position = 0)
{
    return pointerPos.movePointer(position) + ">>[.>]<[<]<";
}

std::string generateBrainfuck()
{
    std::string finalCode = "";

    // Initial setup
    std::cout << "Setting up init variables...\r\n";

    for(VarEntry<std::string>* entry : cstringTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        finalCode += pointerPos.movePointer(entry->memoryPosition);
        finalCode += std::string(entry->initialValue.size(), '+');
        finalCode += pointerPos.relativePointer(2);
        for(int i = 0; i < entry->initialValue.size(); i++)
        {
            std::cout << "character " << i << ": " << (int)entry->initialValue[i] << "\r\n";
            finalCode += std::string((int)entry->initialValue[i], '+');
            finalCode += pointerPos.relativePointer(1);
        }
    }

    for(VarEntry<int>* entry : integerTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        finalCode += pointerPos.movePointer(entry->memoryPosition);
        finalCode += std::string(entry->initialValue, '+');
    }

    for(VarEntry<std::string>* entry : stringTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        finalCode += pointerPos.movePointer(entry->memoryPosition);
        finalCode += std::string(entry->initialValue.size(), '+');
        finalCode += pointerPos.relativePointer(2);
        for(int i = 0; i < entry->initialValue.size(); i++)
        {
            std::cout << "character " << i << ": " << (int)entry->initialValue[i] << "\r\n";
            finalCode += std::string((int)entry->initialValue[i], '+');
            finalCode += pointerPos.relativePointer(1);
        }
    }

    finalCode += start->process();

    //std::cout << "Currentpos: " << pointerPos.getPointer() << "\r\n";
    //finalCode += printMessage(17);
    //std::cout << "Currentpos: " << pointerPos.getPointer() << "\r\n";
    //finalCode += printMessage(2);

    return finalCode;
}