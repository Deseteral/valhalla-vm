#pragma once

#include <vector>
#include "../data_types.h"

class Assembler
{
public:
    Assembler(string filePath);
    ~Assembler();

    void compile();

private:
    std::vector<string> fileLines;
    std::vector<u8> bytecode;

    void logError(string message, uint lineNumber, string line);
    std::vector<string> tokensFromLine(string line);
};