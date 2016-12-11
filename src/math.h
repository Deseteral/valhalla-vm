#pragma once

#include "data_types.h"

static inline u16 bytesToShort(u8 a, u8 b)
{
    u16 out = a;
    out += (b << 8);
    return  out;
}

static inline void shortToBytes(u16 a, u8* out)
{
    out[0] = a & 0xff;
    out[1] = (a >> 8);
}
