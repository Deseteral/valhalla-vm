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
};