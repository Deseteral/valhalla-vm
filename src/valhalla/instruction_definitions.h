#pragma once

#include "../data_types.h"

#define INSTRUCTION_VALUE_REGISTER 0x1
#define INSTRUCTION_VALUE_LITERAL  0x2
#define INSTRUCTION_VALUE_ADDRESS  0x3

struct InstructionDefinition
{
    string token;
    u8 byte;
    uint argumentCount;
};

#define INSTRUCTION_COUNT 4
static InstructionDefinition instructionDefinitions[] = {
    //                     token| byte| argumentCount|
    InstructionDefinition{ "nop",  0x0,             0 },
    InstructionDefinition{ "hcf",  0x1,             0 },
    InstructionDefinition{ "set",  0x2,             2 },
    InstructionDefinition{ "add",  0x3,             2 }
};

static InstructionDefinition* findInstructionDefinitionByToken(string token)
{
    for (int i = 0; i < INSTRUCTION_COUNT; i++)
    {
        if (token == instructionDefinitions[i].token)
            return &instructionDefinitions[i];
    }

    return NULL;
}
