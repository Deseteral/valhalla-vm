#pragma once

#include "../data_types.h"

struct RegisterDefinition
{
    string name;
    u8 byte;
};

#define REGISTER_COUNT 6
static RegisterDefinition registerDefinitions[] = {
    RegisterDefinition{ "X", 0x1 },
    RegisterDefinition{ "Y", 0x2 },
    RegisterDefinition{ "C", 0x3 },
    RegisterDefinition{ "W", 0x4 },
    RegisterDefinition{ "A", 0x5 },
    RegisterDefinition{ "B", 0x6 }
};

static bool isTokenRegister(string token)
{
    if (token.length() != 1)
        return false;

    for (uint i = 0; i < REGISTER_COUNT; i++)
    {
        if (registerDefinitions[i].name == token)
            return true;
    }

    return false;
}

static u8 findRegisterByte(string registerName)
{
    for (uint i = 0; i < REGISTER_COUNT; i++)
    {
        if (registerDefinitions[i].name == registerName)
            return registerDefinitions[i].byte;
    }
}
