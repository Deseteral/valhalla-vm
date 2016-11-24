#include "Assembler.h"

#include <algorithm>
#include <iostream>
#include <fstream>

#include "instruction_definitions.h"

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
    std::cout << "Started compilation" << std::endl;
    this->bytecode.clear();

    for (int i = 0; i < fileLines.size(); i++)
    {
        string currentLine = fileLines[i];
        std::vector<string> tokens = tokensFromLine(currentLine);

        std::cout << "Line: <" << currentLine << ">" << std::endl;
        std::cout << "Tokens: " << std::endl;

        for (int k = 0; k < tokens.size(); k++)
            std::cout << "  <" << tokens[k] << ">" << std::endl;

        if (tokens.size() > 0)
        {
            string tokenLowerCase = tokens[0];
            std::transform(
                tokenLowerCase.begin(),
                tokenLowerCase.end(),
                tokenLowerCase.begin(),
                ::tolower
            );

            InstructionDefinition* definition =
                findInstructionDefinitionByToken(tokenLowerCase);

            if (definition != NULL)
            {
                std::cout << "Byte: " << (int)definition->byte << std::endl;
                bytecode.push_back(definition->byte);
            }
            else
            {
                std::cout << "Token <" << tokens[0] << "> doesn't exist" << std::endl;
            }
        }

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