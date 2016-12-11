#include "VM.h"

#include <iostream>
#include <cstring>

#include "instruction_definitions.h"
#include "register_definitions.h"
#include "../math.h"

#define IF_TOKEN(TOKEN) if (currentInstruction->token == TOKEN)
#define LOAD_VALUE_A_B \
    u8 valueAType = memory[pc++]; \
    u8 valueA, valueB; \
    u8* saveAddress = NULL; \
    \
    if (valueAType == INSTRUCTION_VALUE_REGISTER) \
    { \
        u8 registerByte = memory[pc++]; \
        \
        saveAddress = &registers[registerByte]; \
        valueA = registers[registerByte]; \
    } \
    else if (valueAType == INSTRUCTION_VALUE_ADDRESS) \
    { \
        u8 addrByte1 = memory[pc++]; \
        u8 addrByte2 = memory[pc++]; \
        \
        u16 memoryAddress = bytesToShort(addrByte1, addrByte2); \
        \
        saveAddress = &memory[memoryAddress]; \
        valueA = memory[memoryAddress]; \
    } \
    \
    u8 valueBType = memory[pc++]; \
    \
    if (valueBType == INSTRUCTION_VALUE_REGISTER) \
    { \
        u8 registerByte = memory[pc++]; \
        \
        valueB = registers[registerByte]; \
    } \
    else if (valueBType == INSTRUCTION_VALUE_LITERAL) \
        valueB = memory[pc++]; \
    else if (valueBType == INSTRUCTION_VALUE_ADDRESS) \
    { \
        u8 addrByte1 = memory[pc++]; \
        u8 addrByte2 = memory[pc++]; \
        \
        u16 memoryAddress = bytesToShort(addrByte1, addrByte2); \
        \
        valueB = memory[memoryAddress]; \
    }


VM::VM(u16 memSize) : halt(false), memorySize(memSize), pc(0)
{
    memory = new u8[memSize];
    registers = new u8[REGISTER_COUNT]();
}

VM::~VM()
{ }

void VM::tick()
{
    std::cout << "registers: ";
    for (int i = 0; i < REGISTER_COUNT; i++)
        std::cout << (uint)registers[i] << " ";
    std::cout << std::endl;

    u8 currentByte = memory[pc++];
    InstructionDefinition* currentInstruction = findInstructionDefinitionByByte(currentByte);

    IF_TOKEN("nop")
    {
        return;
    }
    else IF_TOKEN("hcf")
    {
        halt = true;
    }
    else IF_TOKEN("set")
    {
        LOAD_VALUE_A_B
        std::memset(saveAddress, valueB, 1);
    }
    else IF_TOKEN("add")
    {
        LOAD_VALUE_A_B
        std::memset(saveAddress, (u8)(valueA + valueB), 1);
    }
    else IF_TOKEN("sub")
    {
        LOAD_VALUE_A_B
            std::memset(saveAddress, (u8)(valueA - valueB), 1);
    }
    else IF_TOKEN("mul")
    {
        LOAD_VALUE_A_B
            std::memset(saveAddress, (u8)(valueA * valueB), 1);
    }
    else IF_TOKEN("div")
    {
        LOAD_VALUE_A_B
            std::memset(saveAddress, (u8)(valueA / valueB), 1);
    }
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
