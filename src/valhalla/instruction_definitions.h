#pragma once

#include "../data_types.h"

#define INSTRUCTION_VALUE_COUNT     2

#define INSTRUCTION_VALUE_NA        0x0
#define INSTRUCTION_VALUE_REGISTER  0x1
#define INSTRUCTION_VALUE_IMMEDIATE 0x2

#define NA  INSTRUCTION_VALUE_NA
#define REG INSTRUCTION_VALUE_REGISTER
#define IMM INSTRUCTION_VALUE_IMMEDIATE

struct InstructionDefinition
{
    string token;
    u8 byte;
    uint valueCount;

    uint a;
    uint b;
};

static InstructionDefinition instructionDefinitions[] = {
    //                     token|  byte| valueCount|   a|    b|
    InstructionDefinition{ "nop",  0x00,          0,  NA,  NA },
    InstructionDefinition{ "hcf",  0x01,          0,  NA,  NA },
    InstructionDefinition{ "mov",  0x02,          2, REG, REG },
    InstructionDefinition{ "set",  0x03,          2, REG, IMM },
    InstructionDefinition{ "get",  0x04,          2, REG, REG },
    InstructionDefinition{ "str",  0x05,          2, REG, REG },
    InstructionDefinition{ "add",  0x06,          2, REG, REG },
    InstructionDefinition{ "sub",  0x07,          2, REG, REG },
    InstructionDefinition{ "mul",  0x08,          2, REG, REG },
    InstructionDefinition{ "div",  0x09,          2, REG, REG },
    InstructionDefinition{ "mod",  0x0A,          2, REG, REG },
    InstructionDefinition{  "or",  0x0B,          2, REG, REG },
    InstructionDefinition{ "and",  0x0C,          2, REG, REG },
    InstructionDefinition{ "xor",  0x0D,          2, REG, REG },
    InstructionDefinition{ "not",  0x0E,          1, REG,  NA },
    InstructionDefinition{ "shl",  0x0F,          2, REG, REG },
    InstructionDefinition{ "shr",  0x10,          2, REG, REG },
    InstructionDefinition{ "pri",  0x11,          1, REG,  NA },
    InstructionDefinition{ "prs",  0x12,          1, IMM,  NA },
    InstructionDefinition{ "cmp",  0x13,          2, REG, REG },
    InstructionDefinition{ "jmp",  0x14,          1, IMM,  NA },
    InstructionDefinition{ "jeq",  0x15,          1, IMM,  NA },
    InstructionDefinition{ "jlt",  0x16,          1, IMM,  NA },
    InstructionDefinition{ "jgt",  0x17,          1, IMM,  NA },
    InstructionDefinition{ "jle",  0x18,          1, IMM,  NA },
    InstructionDefinition{ "jge",  0x19,          1, IMM,  NA },
    InstructionDefinition{ "jne",  0x1A,          1, IMM,  NA },
    InstructionDefinition{ "red",  0x1B,          1, REG,  NA },
    InstructionDefinition{ "rnd",  0x1C,          1, REG,  NA },
    // update INSTRUCTION_COUNT!
};
#define INSTRUCTION_COUNT 29

static InstructionDefinition* findInstructionDefinitionByToken(string token)
{
    for (int i = 0; i < INSTRUCTION_COUNT; i++)
    {
        if (token == instructionDefinitions[i].token)
            return &instructionDefinitions[i];
    }

    return NULL;
}

static InstructionDefinition* findInstructionDefinitionByByte(u8 byte)
{
    for (int i = 0; i < INSTRUCTION_COUNT; i++)
    {
        if (byte == instructionDefinitions[i].byte)
            return &instructionDefinitions[i];
    }

    return NULL;
}
