# Valhalla VM spec

`rd` = destination register,
`rs` = source register,
`im` = immediate value

Value | Opcode      | Description
------|-------------|----------------------------------------------------------------------
0x00  | `NOP`       | Do nothing.
0x01  | `HCF`       | Halt and catch fire. Stops execution of instructions.
0x02  | `MOV rd rs` | Sets `rd` to the value of `rs`.
0x03  | `SET rd im` | Sets `rd` to `im`.
0x04  | `GET rd rs` | Sets `rd` to the value of memory at location `rs`.
0x05  | `STR rd rs` | Sets the memory at location `rd` to the value of `rs`.
0x06  | `ADD rd rs` | Adds the values of `rd` and `rs` and stores the result in `rd`.
0x07  | `SUB rd rs` | Subtracts `rs` from `rd` and stores the result in `rd`.
0x08  | `MUL rd rs` | Multiplies the values of `rd` and `rs` and stores the result in `rd`.
0x09  | `DIV rd rs` | Divides `rd` by `rs` and stores the result in `rd`.
0x0A  | `MOD rd rs` | Divides `rd` by `rs` and stores the modulo in `rd`.
0x0B  | `OR  rd rs` | Sets `rd` to bitwise or of `rd` and `rs`.
0x0C  | `AND rd rs` | Sets `rd` to bitwise and of `rd` and `rs`.
0x0D  | `XOR rd rs` | Sets `rd` to bitwise xor of `rd` and `rs`.
0x0E  | `NOT rd`    | Negates `rd`.
0x0F  | `SHL rd rs` | Left shifts bits stored in `rd` by `rs` positions.
0x10  | `SHR rd rs` | Right shifts bits stored in `rd` by `rs` positions.
