#ifndef __MINI_CPU__
#define __MINI_CPU__

#include <cstdint>

enum Registers
{
    REG_0 = 0,
    REG_1 = 1,
    REG_2 = 2,
    REG_3 = 3,
    REG_4 = 4,
    REG_5 = 5,
    REG_6 = 6,
    REG_7 = 7,
    REG_8 = 8,
    REG_9 = 9,
    REG_10 = 10,
    REG_11 = 11,
    REG_12 = 12,
    REG_13 = 13,
    REG_14 = 14,
    REG_C = 15, // The special constant register, value is the constant part of the instruction.

    NUMBER_REGISTERS,
    REG_IS_ADDRESS = 128 // top bit says it's an address.
};

union Register
{
    int8_t  s8;
    int16_t s16;
    int32_t s32;
    int64_t s64;

    u_int8_t  u8;
    u_int16_t u16;
    u_int32_t u32;
    u_int64_t u64;

    int8_t  *ps8;
    int16_t *ps16;
    int32_t *ps32;
    int64_t *ps64;

    u_int8_t  *pu8;
    u_int16_t *pu16;
    u_int32_t *pu32;
    u_int64_t *pu64;
};

    // The assember format is the same as the instruction format, 5 elements. Operation Type,Source,Destination,Constant.
    // For all instructions even if not used. Makes generating the 32bit instuction value a lot easier.
    // The only 'special' things I do allow is:-
    //   the use of I for source to say use the constant data
    //   the & simbol to state register is an address not value
    //   the - symbol for ignored params
    //   PC for source register for some instrutions.
    //   source can be comparison flags for some instructions.
    // These 'special' things that may change which variant of instruction is used.
    // Constant is always a HEX. Macro assember will take what ever you want to use and make it correct.
    // We'll have a macro assembler to create the assembler code from something easier to write.
    //      For example it will take MOVE 0x12345678ABCDEF,R1 and generate
    //          MOVE U64,I0,R1,0x1234
    //          MOVE U64,I1,R1,0x4567
    //          MOVE U64,I2,R1,0x89AB
    //          MOVE U64,I3,R1,0xCDEF
    // You'll be expected to use a macro assembler because to try to write the assember directly else would be daft! ;)
    // But I just need to get this low level compiling seperated out to make life a lot easier.
    // Gets messy otherwise. Remember, seperation of concerns.



#define OPERATION_CODES               \
    MAKE_OPCODE("MOVE",OP_MOVE)       \
    /* Copies from the source register to the dest register. Number bytes copied is the data type * the unsigned constant data. */  \
    MAKE_OPCODE("MEMSET",OP_MEMSET)      /* MEMSET U32,R0,&R1,0x10 /* Copy 16 32 bit values, 128 bytes, to address. */             \
    MAKE_OPCODE("MEMCPY",OP_MEMCPY)      /* MEMCPY U32,&R0,&R1,0x10 /* Copy 16 32 bit values, 128 bytes, to address.  */           \
    /* Stack operations. */                                                                                                         \
    MAKE_OPCODE("POP",OP_POP)       /* POP -,-,R0,-    Move 32 bit value from stack into register, decrement SP */\
    MAKE_OPCODE("PUSH",OP_PUSH)      /* PUSH -,R0,-,-   Move 32Bit value from register into stack. */\
    MAKE_OPCODE("SPSET",OP_SPSET)       /* SPSET -,R0,-,-  Sets the stack pointer to the value of the passed register     */\
    MAKE_OPCODE("SPGET",OP_SPGET)       /* SPGET -,-,R0,-  Sets the register to the value of the stack pointer */\
    MAKE_OPCODE("SAVE",OP_SAVE)        /* SAVE -,-,-,-   Saves everything (state) that needs to be saved to stack. */\
    MAKE_OPCODE("LOAD",OP_LOAD)        /* LOAD -,-,-,-   Loads everything (state) that was saved on the stack. */\
    /* Program control */\
    MAKE_OPCODE("CMP",OP_CMP)       /* CMP U32,R1,R2,- */\
    MAKE_OPCODE("JUMP",OP_JUMP)       /* JUMP 1,NE,R15,0xfffc  Jump back 3 instructions. Jump to address in register, constant is signed 16 bit. First parameter states if PC relative (true) or absolute (false). Jump is always N instructions. Either from address 0 or from PC. This means instructions are always 4 byte aligned. */\
    MAKE_OPCODE("RET",OP_RET)          /* RET -,-,-,-       Sets PC to value in stack. Also used for returning from interupts as before an interupt is called PC is pushed to stack. */\
    MAKE_OPCODE("SWAP",OP_SWAP)      /* SWAP S32,R1,R2,-  Atomic value swap needed to allow semaphores and threading. */\
    MAKE_OPCODE("PAUSE",OP_PAUSE)        /* PAUSE S32,R1,-,-  Pause micro seconds. */\
    MAKE_OPCODE("SETINT",OP_SETINT)      /* SETINT -,R1,-,-    Enable specific interupts. */\
    MAKE_OPCODE("CLRINT",OP_CLRINT)      /* CLRINT -,R1,-,-    Disable specific interupts. */\
    /* Bit wise operations */\
    MAKE_OPCODE("OR",OP_OR)       /* OR S32,R1,R2,-     */\
    MAKE_OPCODE("XOR",OP_XOR)      /* XOR S32,R1,R2,-   */\
    MAKE_OPCODE("AND",OP_AND)      /* AND S32,R1,R2,- */\
    MAKE_OPCODE("NOT",OP_NOT)      /* NOT S32,R1,R2,-   */\
    /* These are to allow easy toggling of control bits in one instruction.     */\
    MAKE_OPCODE("SETBIT",OP_SETBIT)       /* SET -,R2,R2,0x01  Takes the source, sets the bit stated in the constant data, puts the result into dest.  */\
    MAKE_OPCODE("CLRBIT",OP_CLRBIT)        /* CLR -,R2,R2,0x01  Takes the source, clears the bit stated in the constant data, puts the result into dest.  */\
    /* Logical shift left / right. shift value in register by N bits. Will only shift the number of bits indicated by type. */\
    /* bits do not carry or role over. So LSR U8 11000011 with give 01100001 */\
    MAKE_OPCODE("LSL",OP_LSL)       /* LSL S32,R1,R2,0x4  */\
    MAKE_OPCODE("LSR",OP_LSR)       /* LSR S32,R1,R2,0x4 */\
    /* Arithmetic shift left / right. shift value in register by N bits. Will only shift the number of bits indicated by type. */\
    /* bits do not carry, role over. So ASR U8 11000011 with give 11100001 */\
    /* There is no ASL */\
    MAKE_OPCODE("ASR",OP_ASR)       /* LSR S32,R1,R2,0x4 */\
    /* Math */\
    /* dest = dest + source */\
    MAKE_OPCODE("ADD",OP_ADD)      /* ADD U32,R1,R1,- */\
    /* dest = dest - source */\
    MAKE_OPCODE("SUB",OP_SUB)    \
    /* dest = dest * source */\
    MAKE_OPCODE("MUL",OP_MUL)    \
    /* dest = dest / source */\
    /* Any integer division by zero returns zero. */\
    MAKE_OPCODE("DIV",OP_DIV)    \
    /* Gets the remainder of a division. */\
    /* dest = remainder of dest / source. 7 = 107 / 10 */\
    /* Any integer division by zero returns zero. */\
    MAKE_OPCODE("DIVR",OP_DIVR)    \
    MAKE_OPCODE("RAND",OP_RAND)      /* RAND U32,-,&R1,0x10 /* Make random 32 bit value and write to address.  */           \
    /* Linear interpolation from source to dest based on constant. Constant treated as 16 unsigned 0 -> 1 value. dest = source + ((dest-source) * constant) */\
    MAKE_OPCODE("LERP",OP_LERP)     /* LERP U32,R1,R2,0x0100 */\
    MAKE_OPCODE("MAX",OP_MAX)         /* MAX S32,R1,R2,- */\
    MAKE_OPCODE("MIN",OP_MIN)         /* MAX S32,R1,R2,- */\
    MAKE_OPCODE("FADD",OP_FADD)    \
    MAKE_OPCODE("FSUB",OP_FSUB)    \
    MAKE_OPCODE("FMUL",OP_FMUL)    \
    MAKE_OPCODE("FDIV",OP_FDIV)    \
    MAKE_OPCODE("FRAC",OP_FRAC)    /* Fractional part of a float or double. */\
    MAKE_OPCODE("FRAND",OP_FRAND)      /* RAND FLOAT,-,&R1,0x10 /* Make random float 0.0 -> 1.0 value and write to address.  */           \
    /* Linear interpolation from source to dest based on constant. Constant treated as 16 unsigned 0 -> 1 value. dest = source + ((dest-source) * constant) */\
    MAKE_OPCODE("FLERP",OP_FLERP)     /* LERP FLOAT,R1,R2,0x0100 */\
    MAKE_OPCODE("FMAX",OP_FMAX)    \
    MAKE_OPCODE("FMIN",OP_FMIN)    \
    MAKE_OPCODE("FSQRT",OP_FSQRT)  \
    MAKE_OPCODE("FSIN",OP_FSIN)    \
    MAKE_OPCODE("FCOS",OP_FCOS)    \
    MAKE_OPCODE("FTAN",OP_FTAN)    \
    MAKE_OPCODE("FATAN",OP_FATAN)    

    
enum OperationCodes
{
#define MAKE_OPCODE(__OP_NAME__,__OP_CODE__)  __OP_CODE__,
    OPERATION_CODES
#undef MAKE_OPCODE

    OP_RESERVED_00,
    OP_RESERVED_01,
    OP_RESERVED_02,
    OP_RESERVED_03,
    OP_RESERVED_04,
    OP_RESERVED_05,
    OP_RESERVED_06,
    OP_RESERVED_07,
    OP_RESERVED_08,
    OP_RESERVED_09,
    OP_RESERVED_10,
    OP_RESERVED_11,
    OP_RESERVED_12,
    OP_RESERVED_13,
    OP_RESERVED_14,
    OP_RESERVED_15,

    OP_LAST = OP_RESERVED_11,

    NUMBER_OPERATIONS
};

enum DataType
{
    DataType_UNSIGNED_INT_8 = 0,
    DataType_UNSIGNED_INT_16 = 1,
    DataType_UNSIGNED_INT_32 = 2,
    DataType_UNSIGNED_INT_64 = 3,
    DataType_SIGNED_INT_8 = 4,
    DataType_SIGNED_INT_16 = 5,
    DataType_SIGNED_INT_32 = 6,
    DataType_SIGNED_INT_64 = 7,

    // Types for float math
    DataType_FLOAT = 0,
    DataType_DOUBLE = 1,

    // Special code for when data type is ignored.
    DataType_IGNORE = 0
};

enum ConditionFlags
{
    ConFlag_Negative = 0,
    ConFlag_Zero = 1,
    ConFlag_Carry = 2,
    ConFlag_Overflow = 3
};

// The reason the first bit is used to differentiate between the special load instuction and the rest of the instuction set
// is to maximize the size of the constant value that can be moved.
// This reduces the number of instructions needed to load values into the registers.
struct __attribute__ ((packed)) Instruction
{
    uint32_t IsLoad:1;  // For this instruction type, always 0.
    uint32_t OpCode:6;

    uint32_t Source:4;
    uint32_t SourceIsAddress:1;
    uint32_t Dest:4;
    uint32_t DestIsAddress:1;
    uint32_t DataType:3;

    uint32_t ConstantData:12;
};

struct __attribute__ ((packed)) LoadInstruction
{
    uint32_t IsLoad:1;          // For this instruction type, always 1.
    uint32_t OrWithDest:1;      // If true, the constant value is or'd against the contents of the destination.
    uint32_t Shift:2;           // The amount to shift, in 24bit increments, the constant before updating dest.
    uint32_t Dest:4;            // The destination register.

    u_int32_t ConstantData:24;  // The all important constant data.
};

struct __attribute__ ((packed)) JumpInstruction
{
    uint32_t IsLoad:1;          // For this instruction type, always 0.
    uint32_t OpCode:6;

    uint32_t Condition:4;
    uint32_t PCRelative:1;      // If true then the value in OffsetRegister is relative to the program counter. If false then is an absolute address.
    uint32_t OffsetRegister:4;  // Always in numbers of instructions, IE multiples of four bytes.

    uint32_t ConstantData:17;   // Signed 17bit value, so +- 64k instructions to jump. 
};

struct AddressSpace
{
    struct
    {
        u_int8_t Reset[0x4000];
        u_int8_t Reset[0x4000];
        u_int8_t Reset[0x4000];
        u_int8_t Reset[0x4000];
    }InteruptCode;

    u_int8_t BootCode[0x000000000000ffff];
    u_int8_t BootCode[0x000000000000ffff];
};

class MiniCPU
{
public:
    MiniCPU();
    ~MiniCPU();

    void Reset();

private:
    u_int8_t mRam[1024*1024];
    Register mRegisters[NUMBER_REGISTERS];

};

#endif //__MINI_CPU__
