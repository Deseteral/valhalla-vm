#pragma once

#include "../data_types.h"

struct InstructionDefinition
{
    string token;
    u8 byte;
    uint argumentCount;
};

#define INSTRUCTION_COUNT 4
static InstructionDefinition definitions[] = {
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
        if (token == definitions[i].token)
            return &definitions[i];
    }

    return NULL;
}
