#include <iostream>
#include <string>
#include <regex>

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

void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
    // Get the first occurrence
    size_t pos = data.find(toSearch);
    // Repeat till end is reached
    while( pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos =data.find(toSearch, pos + replaceStr.size());
    }
}

std::string beautify(std::string finalCode)
{
    std::regex movReg("([>]+[<]+|[<]+[>]+)+[<>]*");
    std::smatch match;
    while(std::regex_search(finalCode, match, movReg))
    {
        int count = 0;
        std::string replace = match[0];
        std::string newString = "";
        for (int i = 0; i < replace.size(); i++)
        {
            if (replace[i] == '>') count++;
            else if(replace[i] == '<') count--;
        }
        if(count > 0)
        {
            newString = std::string(count, '>');
        }
        else
        {
            newString = std::string(-count, '<');
        }
        findAndReplaceAll(finalCode, replace, newString);
    }

    std::regex addReg("([+]+[-]+|[-]+[+]+)+[+-]*");
    while(std::regex_search(finalCode, match, addReg))
    {
        int count = 0;
        std::string replace = match[0];
        std::string newString = "";
        for (int i = 0; i < replace.size(); i++)
        {
            if (replace[i] == '+') count++;
            else if(replace[i] == '-') count--;
        }
        if(count > 0)
        {
            newString = std::string(count, '+');
        }
        else
        {
            newString = std::string(-count, '-');
        }
        findAndReplaceAll(finalCode, replace, newString);
    }

    return finalCode;
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
    finalCode += "Setting up global variables\r\n\r\n";

    for(VarEntry<std::string>* entry : cstringTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        finalCode += entry->varName + ": ";
        finalCode += pointerPos.movePointer(entry->memoryPosition);
        finalCode += std::string(entry->initialValue.size(), '+');
        finalCode += pointerPos.relativePointer(2);
        for(int i = 0; i < entry->initialValue.size(); i++)
        {
            std::cout << "character " << i << ": " << (int)entry->initialValue[i] << "\r\n";
            finalCode += std::string((int)entry->initialValue[i], '+');
            finalCode += pointerPos.relativePointer(1) + "\r\n";
        }
    }

    for(VarEntry<int>* entry : integerTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        finalCode += entry->varName + ": ";
        finalCode += pointerPos.movePointer(entry->memoryPosition);
        finalCode += std::string(entry->initialValue, '+') + "\r\n";
    }

    for(VarEntry<std::string>* entry : stringTable)
    {
        std::cout << "Entry: " << *entry << "\r\n";
        finalCode += entry->varName + ": ";
        finalCode += pointerPos.movePointer(entry->memoryPosition);
        finalCode += std::string(entry->initialValue.size(), '+');
        finalCode += pointerPos.relativePointer(2);
        for(int i = 0; i < entry->initialValue.size(); i++)
        {
            std::cout << "character " << i << ": " << (int)entry->initialValue[i] << "\r\n";
            finalCode += std::string((int)entry->initialValue[i], '+');
            finalCode += pointerPos.relativePointer(1) + "\r\n";
        }
    }

    finalCode += "\r\n\r\n";

    finalCode += start->process();

    finalCode = beautify(finalCode);

    //std::cout << "Currentpos: " << pointerPos.getPointer() << "\r\n";
    //finalCode += printMessage(17);
    //std::cout << "Currentpos: " << pointerPos.getPointer() << "\r\n";
    //finalCode += printMessage(2);

    return finalCode;
}