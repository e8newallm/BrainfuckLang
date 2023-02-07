#include <iostream>
#include <string>
#include <regex>

#include "metadata.h"
#include "parsetree/parsetree.h"
#include "process.h"

int maxSumLevels = 0;
int currentSumLevels = 0;

std::vector<VarEntry<int>*> integerTable;
std::vector<VarEntry<std::string>*> stringTable;
std::vector<VarEntry<std::string>*> cstringTable;
std::vector<std::string> varNamesTable;
ParseTree* start;

std::string movRight(uint64_t delta)
{
    return std::string(delta, '>');
}

std::string movLeft(uint64_t delta)
{
    return std::string(delta, '<');
}

std::string movTmp(std::string scope, uint64_t delta)
{
    return movRight(delta) + scope + movLeft(delta);
}

std::string inc(uint64_t amount)
{
    return std::string(amount, '+');
}

std::string dec(uint64_t amount)
{
    return std::string(amount, '-');
}

void processData()
{
    int positionAssigner = 0;

    for(VarEntry<int>* entry : integerTable)
    {
        entry->memoryPosition = positionAssigner;
        positionAssigner += 10;
    }

    positionAssigner++; //null character for start of string

    for(VarEntry<std::string>* entry : cstringTable)
    {
        entry->memoryPosition = positionAssigner;
        positionAssigner += entry->initialValue.size();
        positionAssigner++;
    }

    for(VarEntry<std::string>* entry : stringTable)
    {
        entry->memoryPosition = positionAssigner;
        positionAssigner += entry->initialValue.size();
        positionAssigner++;
    }
}

void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr)
{
    size_t pos = data.find(toSearch);
    while(pos != std::string::npos)
    {
        data.replace(pos, toSearch.size(), replaceStr);
        pos = data.find(toSearch, pos + replaceStr.size());
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
            newString = movRight(count);
        }
        else
        {
            newString = movLeft(-count);
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
    return movTmp("[.>]<[<]>", position);
}

std::string generateBrainfuck()
{
    std::string finalCode = "";

    // Initial setup
    finalCode += "Setting up global variables\r\n\r\n";

    for(VarEntry<int>* entry : integerTable)
    {
        char intChars[10];
        for(int i = 0; i < 10; i++) intChars[i] = 0;
        
        finalCode += "integer variable\"" + entry->varName + "\": ";
        finalCode += movTmp(std::string(entry->initialValue, '+'), entry->memoryPosition) + "\r\n";
    }

    for(VarEntry<std::string>* entry : cstringTable)
    {
        std::string tempCode = "";
        tempCode += "\r\nstring const \"" + entry->varName + "\": ";
        for(int i = 0; i < entry->initialValue.size(); i++)
        {
            tempCode += std::string((int)entry->initialValue[i], '+');
            tempCode += movRight(1) + " " + entry->initialValue[i] + "\r\n";
        }
        tempCode += movLeft(entry->initialValue.size());
        finalCode += movTmp(tempCode, entry->memoryPosition);
    }

    for(VarEntry<std::string>* entry : stringTable)
    {
        std::string tempCode = "";
        tempCode += "\r\nstring variable \"" + entry->varName + "\": ";
        for(int i = 0; i < entry->initialValue.size(); i++)
        {
            tempCode += std::string((int)entry->initialValue[i], '+');
            tempCode += movRight(1) + " " + entry->initialValue[i] + "\r\n";
        }
        tempCode += movLeft(entry->initialValue.size());
        finalCode += movTmp(tempCode, entry->memoryPosition);
    }

    finalCode += "\r\n\r\n";
    finalCode += start->process();
    finalCode = beautify(finalCode);
    return finalCode;
}