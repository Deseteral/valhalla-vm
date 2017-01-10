#include "VM.h"

#include <iostream>
#include <cstring>
#include <sstream>

#include "instruction_definitions.h"
#include "register_definitions.h"

#define IF_TOKEN(TOKEN) if (currentInstruction->token == TOKEN)

#define READ_VALUE_A \
    u8 registerByteA = memory[pc++]; \
    \
    u8* saveAddress = &registers[registerByteA]; \
    u8 valueA = registers[registerByteA];

#define READ_VALUE_A_IMMEDIATE \
    u8 valueA = memory[pc++];

#define READ_VALUE_B_REGISTER \
    u8 registerByteB = memory[pc++]; \
    u8 valueB = registers[registerByteB];

#define READ_VALUE_B_IMMEDIATE \
    u8 valueB = memory[pc++];

#define VALUE_CMP_EQ 0
#define VALUE_CMP_LT 1
#define VALUE_CMP_GT 2

VM::VM(VMConfig config) : halt(false), pc(0)
{
    memorySize = config.memorySize;
    memory = new u8[memorySize];
    registers = new u8[REGISTER_COUNT]();
    display = new Display(config.displayWidth, config.displayHeight);
}

VM::~VM()
{
    delete display;
}

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
    else IF_TOKEN("mov")
    {
        READ_VALUE_A;
        READ_VALUE_B_REGISTER;

        std::memset(saveAddress, valueB, 1);
    }
    else IF_TOKEN("set")
    {
        READ_VALUE_A;
        READ_VALUE_B_IMMEDIATE;

        std::memset(saveAddress, valueB, 1);
    }
    else IF_TOKEN("add")
    {
        READ_VALUE_A;
        READ_VALUE_B_REGISTER;

        std::memset(saveAddress, (u8)(valueA + valueB), 1);
    }
    else IF_TOKEN("sub")
    {
        READ_VALUE_A;
        READ_VALUE_B_REGISTER;
        
        std::memset(saveAddress, (u8)(valueA - valueB), 1);
    }
    else IF_TOKEN("mul")
    {
        READ_VALUE_A;
        READ_VALUE_B_REGISTER;

        std::memset(saveAddress, (u8)(valueA * valueB), 1);
    }
    else IF_TOKEN("div")
    {
        READ_VALUE_A;
        READ_VALUE_B_REGISTER;

        std::memset(saveAddress, (u8)(valueA / valueB), 1);
    }
    else IF_TOKEN("mod")
    {
        READ_VALUE_A;
        READ_VALUE_B_REGISTER;
        
        std::memset(saveAddress, (u8)(valueA % valueB), 1);
    }
    else IF_TOKEN("or")
    {
        READ_VALUE_A;
        READ_VALUE_B_REGISTER;

        std::memset(saveAddress, (u8)(valueA | valueB), 1);
    }
    else IF_TOKEN("and")
    {
        READ_VALUE_A;
        READ_VALUE_B_REGISTER;

        std::memset(saveAddress, (u8)(valueA & valueB), 1);
    }
    else IF_TOKEN("xor")
    {
        READ_VALUE_A;
        READ_VALUE_B_REGISTER;

        std::memset(saveAddress, (u8)(valueA ^ valueB), 1);
    }
    else IF_TOKEN("not")
    {
        READ_VALUE_A;

        std::memset(saveAddress, (u8)(~valueA), 1);
    }
    else IF_TOKEN("shl")
    {
        READ_VALUE_A;
        READ_VALUE_B_REGISTER;

        std::memset(saveAddress, (u8)(valueA << valueB), 1);
    }
    else IF_TOKEN("shr")
    {
        READ_VALUE_A;
        READ_VALUE_B_REGISTER;

        std::memset(saveAddress, (u8)(valueA >> valueB), 1);
    }
    else IF_TOKEN("pri")
    {
        READ_VALUE_A;

        // get display position from XY registers
        uint xpos = registers[findRegisterByte("X")];
        uint ypos = registers[findRegisterByte("Y")];
        uint bufferIndex = ((xpos + ypos * (display->width / 8)) % display->bufferSize);

        // convert value to string
        std::ostringstream ss;
        ss << (uint)valueA;
        string str = ss.str();

        // put characters into display buffer
        for (int i = 0; i < str.length(); i++)
        {
            display->buffer[bufferIndex] = str[i];
            bufferIndex = ((bufferIndex + 1) % display->bufferSize);
        }

        registers[findRegisterByte("X")] = bufferIndex % (display->width / 8);
        registers[findRegisterByte("Y")] = bufferIndex / (display->width / 8);
    }
    else IF_TOKEN("prs")
    {
        READ_VALUE_A_IMMEDIATE;

        // get display position from XY registers
        uint xpos = registers[findRegisterByte("X")];
        uint ypos = registers[findRegisterByte("Y")];
        uint bufferIndex = ((xpos + ypos * (display->width / 8)) % display->bufferSize);

        u8 returnAddress = pc;
        pc = valueA;

        char c = memory[pc++];
        while (c != 0)
        {
            display->buffer[bufferIndex] = c;
            bufferIndex = ((bufferIndex + 1) % display->bufferSize);
            c = memory[pc++];
        }

        registers[findRegisterByte("X")] = bufferIndex % (display->width / 8);
        registers[findRegisterByte("Y")] = bufferIndex / (display->width / 8);

        pc = returnAddress;
    }
    else IF_TOKEN("cmp")
    {
        READ_VALUE_A;
        READ_VALUE_B_REGISTER;

        if (valueA == valueB)
            cmp = VALUE_CMP_EQ;
        else if (valueA < valueB)
            cmp = VALUE_CMP_LT;
        else if (valueA > valueB)
            cmp = VALUE_CMP_GT;
    }
    else IF_TOKEN("jmp")
    {
        READ_VALUE_A_IMMEDIATE;
        this->pc = valueA;
    }
    else IF_TOKEN("jeq")
    {
        READ_VALUE_A_IMMEDIATE;

        if (cmp == VALUE_CMP_EQ)
            this->pc = valueA;
    }
    else IF_TOKEN("jlt")
    {
        READ_VALUE_A_IMMEDIATE;

        if (cmp == VALUE_CMP_LT)
            this->pc = valueA;
    }
    else IF_TOKEN("jgt")
    {
        READ_VALUE_A_IMMEDIATE;

        if (cmp == VALUE_CMP_GT)
            this->pc = valueA;
    }
    else IF_TOKEN("jle")
    {
        READ_VALUE_A_IMMEDIATE;

        if (cmp == VALUE_CMP_LT || cmp == VALUE_CMP_EQ)
            this->pc = valueA;
    }
    else IF_TOKEN("jge")
    {
        READ_VALUE_A_IMMEDIATE;

        if (cmp == VALUE_CMP_GT || cmp == VALUE_CMP_EQ)
            this->pc = valueA;
    }
    else IF_TOKEN("jne")
    {
        READ_VALUE_A_IMMEDIATE;

        if (cmp != VALUE_CMP_EQ)
            this->pc = valueA;
    }
}

void VM::loadIntoMemory(std::vector<u8>* payload)
{
    std::cout << "Loading " << payload->size() << " bytes of bytecode into " <<
        (uint)memorySize << " byte memory" << std::endl;

    if (payload->size() > memorySize)
    {
        std::cout << "Payload size is bigger than memory size!" << std::endl;
        return;
    }

    for (uint i = 0; i < payload->size(); i++)
        memory[i] = payload->at(i);

    // set pc to main entry point
    pc = memory[0];
}
