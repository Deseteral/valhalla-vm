# Valhalla VM spec

## Instructions
All address values are always 16 bit.

Value | Opcode | Description
-|-|-
0x00 | `NOP` | Do nothing.
0x01 | `HCF` | Halt and catch fire. Stops execution of instructions.
0x02 | `SET a b` | Sets `a` to `b`.
0x03 | `ADD a b` | Adds `a` to `b` and stores the result in `a`.
