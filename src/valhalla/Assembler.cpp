#include "Assembler.h"

#include <iostream>
#include <fstream>

Assembler::Assembler(string filePath)
{
    std::ifstream programFileStream(filePath);
    if (!programFileStream.is_open())
        std::cerr << "Couldn't open " + filePath + " file" << std::endl;

    string tmpLine;
    while (std::getline(programFileStream, tmpLine))
        fileLines.push_back(tmpLine);
}

Assembler::~Assembler()
{
}

void Assembler::compile()
{
    for (int i = 0; i < fileLines.size(); i++)
    {
        string line = fileLines[i];
        std::vector<string> tokens = tokensFromLine(line);

        std::cout << "Line: <" << line << ">" << std::endl;
        std::cout << "Tokens: " << std::endl;

        for (int k = 0; k < tokens.size(); k++)
            std::cout << "  <" << tokens[k] << ">" << std::endl;

        std::cout << std::endl << std::endl;
    }
}

std::vector<string> Assembler::tokensFromLine(string line)
{
    std::vector<string> tokens;
    string tmp = "";

    for (int i = 0; i < line.length(); i++)
    {
        char currentChar = line[i];

        if (currentChar == ' ')
        {
            tokens.push_back(tmp);
            tmp = "";
        }
        else if (currentChar == ';')
            break;
        else
            tmp += currentChar;
    }

    if (tmp != "")
        tokens.push_back(tmp);

    return tokens;
}