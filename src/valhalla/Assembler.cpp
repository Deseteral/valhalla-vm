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

void Assembler::logError(string message, uint lineNumber, string line)
{
    std::cout <<
        "Compilation error on line " << lineNumber << ": " << message << std::endl <<
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

                if ((tokens.size() - 1) < definition->argumentCount)
                    LOG_ERROR("Too few arguments");
                else if ((tokens.size() - 1) > definition->argumentCount)
                    LOG_ERROR("Too many arguments");
            }
            else
            {
                LOG_ERROR("Token <" + tokens[0] + "> doesn't exist");
            }
        }
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