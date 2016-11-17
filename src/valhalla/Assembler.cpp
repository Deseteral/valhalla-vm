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

}
