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
        positionAssigner+=entry->initialValue.size();
        positionAssigner++;
    }
}

int currentPos = 0;

std::string printMessage(int position = 0)
{
    std::cout << "printMessage: currentPos: " << currentPos << " position: " << position << "\r\n";
    
    if(currentPos > position)
    {
        std::cout << "currentPos - position: " << currentPos - position << "\r\n";
        return std::string(currentPos - position, '<') + "[.>]";
    }
    else
    {
        std::cout << "position - currentPos: " << position - currentPos << "\r\n";
        return std::string(position - currentPos, '>') + "[.>]";
    }
}

std::string generateBrainfuck()
{
    std::string finalCode = "";

    // Initial setup
    std::cout << "Setting up init variables...\r\n";
    for(varEntry<std::string>* entry : stringTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        finalCode += std::string(entry->getPosition() - currentPos, '>');
        currentPos += entry->getPosition() - currentPos;
        for(int i = 0; i < entry->initialValue.size(); i++)
        {
            std::cout << "character " << i << ": " << (int)entry->initialValue[i] << "\r\n";
            finalCode += std::string((int)entry->initialValue[i], '+');
            finalCode += ">";
            currentPos++;
        }
        
    }

    finalCode += printMessage(15);

    return finalCode;
}