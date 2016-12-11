#include "VM.h"

#include <iostream>
#include "instruction_definitions.h"

VM::VM(u16 memSize) : halt(false), memorySize(memSize), pc(0)
{
    memory = new u8[memSize];
}

VM::~VM()
{ }

void VM::tick()
{
    u8 currentByte = memory[pc++];
    std::cout << currentByte << " :: ";

    if (currentByte == findInstructionDefinitionByToken("hcf")->byte)
        halt = true;
}

void VM::loadIntoMemory(std::vector<u8>* payload)
{
    std::cout << "Loading " << payload->size() << " bytes of bytecode into " <<
        memorySize << " byte memory" << std::endl;

    if (payload->size() > memorySize)
    {
        std::cout << "Payload size is bigger than memory size!" << std::endl;
        return;
    }

    for (uint i = 0; i < payload->size(); i++)
        memory[i] = payload->at(i);
}
