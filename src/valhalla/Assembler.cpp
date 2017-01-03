#include "Assembler.h"

#include <algorithm>
#include <iostream>
#include <fstream>

#include "instruction_definitions.h"
#include "register_definitions.h"

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
{ }

void Assembler::logError(string message, uint lineNumber, string line)
{
    std::cout <<
        "Error on line " << lineNumber << ": " << message << std::endl <<
        "  " << line << std::endl <<
        std::endl;
}

#define LOG_ERROR(message) logError(message, (currentLineNumber + 1), currentLine)

void Assembler::compile()
{
    std::cout << "Started compilation" << std::endl;
    this->bytecode.clear();

    for (int currentLineNumber = 0; currentLineNumber < fileLines.size(); currentLineNumber++)
    {
        string currentLine = fileLines[currentLineNumber];
        std::vector<string> tokens = tokensFromLine(currentLine);

        std::cout << "TOKENS: ";
        for (int h = 0; h < tokens.size(); h++)
            std::cout << "<" << tokens[h] << "> ";

        std::cout << std::endl;

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
                bytecode.push_back(definition->byte);

                if ((tokens.size() - 1) < definition->valueCount)
                    LOG_ERROR("Too few arguments");
                else if ((tokens.size() - 1) > definition->valueCount)
                    LOG_ERROR("Too many arguments");
                else
                {
                    // ignore INSTRUCTION_VALUE_NA

                    if (definition->valueCount >= 1)
                    {
                        string firstToken = tokens[1];

                        if (definition->a == INSTRUCTION_VALUE_REGISTER)
                            bytecode.push_back(findRegisterByte(firstToken));
                        else if (definition->a == INSTRUCTION_VALUE_IMMEDIATE)
                        {
                            u8 value = (u8)std::stoi(firstToken);
                            bytecode.push_back(value);
                        }
                    }

                    if (definition->valueCount >= 2)
                    {
                        string secondToken = tokens[2];

                        if (definition->b == INSTRUCTION_VALUE_REGISTER)
                            bytecode.push_back(findRegisterByte(secondToken));
                        else if (definition->b == INSTRUCTION_VALUE_IMMEDIATE)
                        {
                            u8 value = (u8)std::stoi(secondToken);
                            bytecode.push_back(value);
                        }
                    }
                }
            }
            else
            {
                LOG_ERROR("Token <" + tokens[0] + "> doesn't exist");
            }
        }
    }

    // end bytecode with stopping hcf
    bytecode.push_back(findInstructionDefinitionByToken("hcf")->byte);
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
            if (tmp != "")
            {
                tokens.push_back(tmp);
                tmp = "";
            }
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

std::vector<u8>* Assembler::getBytecode()
{
    return &bytecode;
}
