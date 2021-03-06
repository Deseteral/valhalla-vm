#include "Assembler.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>

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
    bytecode.clear();

    // starting byte (main entry point)
    bytecode.push_back(0);

    std::map<string, u8> labels;
    std::map<uint, string> missingLabels;

    for (uint currentLineNumber = 0; currentLineNumber < fileLines.size(); currentLineNumber++)
    {
        string currentLine = fileLines[currentLineNumber];
        std::vector<string> tokens = tokensFromLine(currentLine);

        std::cout << "TOKENS: ";
        for (uint h = 0; h < tokens.size(); h++)
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

            // if it is a label
            if (tokenLowerCase[0] == ':')
            {
                string labelName = tokenLowerCase.substr(1);

                if (labels.find(labelName) != labels.end())
                    LOG_ERROR("Label: '" + labelName + "' is already defined!");

                labels[labelName] = (u8)bytecode.size();
                continue;
            }

            // it it is a data label
            if (tokenLowerCase[0] == '.')
            {
                string labelName = tokenLowerCase.substr(1);

                if (labels.find(labelName) != labels.end())
                    LOG_ERROR("Data label: '" + labelName + "' is already defined!");

                if (tokens.size() < 2)
                    LOG_ERROR("There is no data on label: " + labelName);

                labels[labelName] = (u8)bytecode.size();

                string data = tokens[1];

                for (uint i = 0; i < data.length(); i++)
                    bytecode.push_back(data[i]);
                bytecode.push_back(0);

                continue;
            }

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
                            // if there is label token (jump instruction or PRS)
                            if (firstToken[0] == ':' || firstToken[0] == '.')
                            {
                                string labelName = firstToken.substr(1);

                                missingLabels[bytecode.size()] = labelName;
                                bytecode.push_back(0);
                            }
                            else
                            {
                                u8 value = (u8)std::stoi(firstToken);
                                bytecode.push_back(value);
                            }
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

    for (
        std::map<uint, string>::iterator it = missingLabels.begin();
        it != missingLabels.end();
        it++)
    {
        uint address = it->first;
        string labelName = it->second;

        if (labels.find(labelName) == labels.end())
            std::cout << "Label: '" + labelName + "' is not defined!" << std::endl;

        bytecode[address] = labels[labelName];
    }

    if (labels.find("start") != labels.end())
        bytecode[0] = labels["start"];

    // end bytecode with stopping hcf
    bytecode.push_back(findInstructionDefinitionByToken("hcf")->byte);
}

std::vector<string> Assembler::tokensFromLine(string line)
{
    std::vector<string> tokens;
    string tmp = "";

    for (uint i = 0; i < line.length(); i++)
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
        else if (tmp == "" && currentChar == '"')
        {
            tokens.push_back(line.substr(i + 1, line.length() - i - 2));
            break;
        }
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

string Assembler::getSourceCode()
{
    string code = "";

    for (uint i = 0; i < fileLines.size(); i++)
    {
        code += fileLines[i];
        code += '\n';
    }

    return code;
}