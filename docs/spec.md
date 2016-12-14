# Valhalla VM spec

## Instructions
All address values are always 16 bit.

Value | Opcode | Description
-|-|-
0x00 | `NOP` | Do nothing.
0x01 | `HCF` | Halt and catch fire. Stops execution of instructions.
0x02 | `SET a b` | Sets `a` to `b`.
0x03 | `ADD a b` | Sets `a` to result of `a+b`.
0x04 | `SUB a b` | Sets `a` to result of `a-b`.
0x05 | `MUL a b` | Sets `a` to result of `a*b`.
0x06 | `DIV a b` | Sets `a` to result of `a/b`.
0x07 | `MOD a b` | Sets `a` to result of `a%b`.
