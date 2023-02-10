#include <iostream>
#include <string>
#include <regex>

#include "metadata.h"
#include "parsetree/parsetree.h"
#include "process.h"
#include "highlevelfuncs.h"

std::vector<VarEntry*> varTable;
std::vector<std::string> varNamesTable;
ParseTree* start;

void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr)
{
    size_t pos = data.find(toSearch);
    while(pos != std::string::npos)
    {
        data.replace(pos, toSearch.size(), replaceStr);
        pos = data.find(toSearch, pos + replaceStr.size());
    }
}

std::string optimise(std::string finalCode)
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

std::string generateBrainfuck()
{
    std::string finalCode = "";

    int positionAssigner = SpecialVarEnd;

    for(VarEntry* entry : varTable)
    {
        switch(entry->valType)
        {
            case number:
            {
                entry->memoryPosition = positionAssigner;
                positionAssigner += 4;
                break;
            }
            case string:
            {
                positionAssigner++; //starting null

                entry->memoryPosition = positionAssigner;
                positionAssigner += entry->initialString.size();
                positionAssigner++; //ending null
            }
        }
    }

    for(VarEntry* entry : varTable)
    {
        switch(entry->valType)
        {
            case number:
            {
                std::string tempCode = "";
                int tempVal = entry->initialNumber;
                tempCode += std::string((entry->initialNumber >> 24) & 0xFF, '+') + movRight(1);
                tempCode += std::string((entry->initialNumber >> 16) & 0xFF, '+') + movRight(1);
                tempCode += std::string((entry->initialNumber >>  8) & 0xFF, '+') + movRight(1);
                tempCode += std::string((entry->initialNumber >>  0) & 0xFF, '+') + movLeft(3);
                finalCode += movTmp(tempCode, entry->memoryPosition);
                break;
            }
            case string:
            {
                std::string tempCode = "";
                for(int i = 0; i < entry->initialString.size(); i++)
                {
                    tempCode += std::string((int)entry->initialString[i], '+');
                    tempCode += movRight(1);
                }
                tempCode += movLeft(entry->initialString.size());
                finalCode += movTmp(tempCode, entry->memoryPosition);
            }
        }
    }

    finalCode += start->process();
    std::string optimisedCode = optimise(finalCode);
    while(optimisedCode != finalCode)
    {
        finalCode = optimisedCode;
        optimisedCode = optimise(finalCode);
    }
    return finalCode;
}