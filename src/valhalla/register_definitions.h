#pragma once

#include "../data_types.h"

struct RegisterDefinition
{
    string name;
    u8 byte;
};

static RegisterDefinition registerDefinitions[] = {
    RegisterDefinition{ "X", 0x0 },
    RegisterDefinition{ "Y", 0x1 },
    RegisterDefinition{ "C", 0x2 },
    RegisterDefinition{ "W", 0x3 },
    RegisterDefinition{ "A", 0x4 },
    RegisterDefinition{ "B", 0x5 },
    RegisterDefinition{ "J", 0x6 },
    RegisterDefinition{ "K", 0x7 },
    RegisterDefinition{ "L", 0x8 },
    // update REGISTER_COUNT!
};
#define REGISTER_COUNT 9

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
