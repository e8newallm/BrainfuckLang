#include <iostream>
#include <string>

#include "metadata.h"

std::vector<varEntry<int>*> integerTable;
std::vector<varEntry<std::string>*> stringTable;

void processData()
{
    int positionAssigner = 0;
    for(varEntry<int>* entry : integerTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        entry->setPosition(positionAssigner);
        positionAssigner++;
    }

    for(varEntry<std::string>* entry : stringTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        std::cout << "string size: " << entry->initialValue.size() << "\r\n";
        entry->setPosition(positionAssigner);
        positionAssigner+=entry->initialValue.size() + 2;
        positionAssigner++;
    }
}

int currentPos = 0;

std::string printMessage(int position = 0)
{
    std::cout << "printMessage: currentPos: " << currentPos << " position: " << position << "\r\n";
    
    if(currentPos > position)
    {
        int distance = currentPos - position;
        currentPos -= distance;
        return std::string(distance, '<') + ">>[.>]<[<]<";
    }
    else
    {
        int distance = position - currentPos;
        currentPos += distance;
        return std::string(distance, '>') + ">>[.>]<[<]<";
    }
}

std::string generateBrainfuck()
{
    std::string finalCode = "";

    // Initial setup
    std::cout << "Setting up init variables...\r\n";

    for(varEntry<int>* entry : integerTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        finalCode += std::string(entry->getPosition() - currentPos, '>');
        currentPos += entry->getPosition() - currentPos;
        finalCode += std::string(entry->initialValue, '+');
    }

    for(varEntry<std::string>* entry : stringTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        finalCode += std::string(entry->getPosition() - currentPos, '>');
        currentPos += entry->getPosition() - currentPos;
        finalCode += std::string(entry->initialValue.size(), '+');
        finalCode += ">>";
        currentPos+= 2;
        for(int i = 0; i < entry->initialValue.size(); i++)
        {
            std::cout << "character " << i << ": " << (int)entry->initialValue[i] << "\r\n";
            finalCode += std::string((int)entry->initialValue[i], '+');
            finalCode += ">";
            currentPos++;
        }
    }
    std::cout << "Currentpos: " << currentPos << "\r\n";
    finalCode += printMessage(17);
    std::cout << "Currentpos: " << currentPos << "\r\n";
    finalCode += printMessage(2);

    return finalCode;
}