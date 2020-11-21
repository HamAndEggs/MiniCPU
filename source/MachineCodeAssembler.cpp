#include <iostream>

#include "MachineCodeAssembler.h"
#include "Util.h"

MachineCodeAssembler::MachineCodeAssembler()
{
    assert( sizeof(Instruction) == sizeof(uint32_t) );
    assert( OP_LAST < 128 );
}

MachineCodeAssembler::~MachineCodeAssembler()
{

}

std::vector<Instruction> MachineCodeAssembler::Compile(const std::string& pAssembler)const
{
    std::vector<Instruction> machineCode;

    const StringVec CodeLines = SplitString(pAssembler,"\n");
    for(const auto &line : CodeLines )
    {
        try
        {
            if( line.size() > 1 )
            {
                const Instruction ins = MakeInstruction(line);
                machineCode.push_back(ins);

                std::cout << ins.OpCode << " " << ins.DataType << " " << ins.Source << " " << ins.Dest << " " << ins.ConstantData << std::endl;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << " : " << line << std::endl;
        }        
    }

    return machineCode;
}

uint32_t MachineCodeAssembler::GetDataType(const std::string& a_Type)const
{
#define DEF_DATA_TYPE(__name__,__value__)   if( CompareNoCase(a_Type,(__name__)) ){return (__value__);}

    DEF_DATA_TYPE("u8",DataType_UNSIGNED_INT_8);
    DEF_DATA_TYPE("u16",DataType_UNSIGNED_INT_16);
    DEF_DATA_TYPE("u32",DataType_UNSIGNED_INT_32);
    DEF_DATA_TYPE("u64",DataType_UNSIGNED_INT_64);
    DEF_DATA_TYPE("s8",DataType_SIGNED_INT_8);
    DEF_DATA_TYPE("s16",DataType_SIGNED_INT_16);
    DEF_DATA_TYPE("s32",DataType_SIGNED_INT_32);
    DEF_DATA_TYPE("s64",DataType_SIGNED_INT_64);

    // Types for float math
    DEF_DATA_TYPE("float",DataType_FLOAT);
    DEF_DATA_TYPE("double",DataType_DOUBLE);

#undef DEF_DATA_TYPE

    return DataType_IGNORE;
}

uint32_t MachineCodeAssembler::GetRegister(const std::string& a_Type)const
{
#define DEF_REGISTER(__name__,__value__)   if( CompareNoCase(a_Type,(__name__)) ){return (__value__);}

    DEF_REGISTER("r0",REG_0);
    DEF_REGISTER("r1",REG_1);
    DEF_REGISTER("r2",REG_2);
    DEF_REGISTER("r3",REG_3);
    DEF_REGISTER("r4",REG_4);
    DEF_REGISTER("r5",REG_5);
    DEF_REGISTER("r6",REG_6);
    DEF_REGISTER("r7",REG_7);
    DEF_REGISTER("r8",REG_8);
    DEF_REGISTER("r9",REG_9);
    DEF_REGISTER("r10",REG_10);
    DEF_REGISTER("r11",REG_11);
    DEF_REGISTER("r12",REG_12);
    DEF_REGISTER("r13",REG_13);
    DEF_REGISTER("r14",REG_14);

    DEF_REGISTER("&r0",REG_0|REG_IS_ADDRESS);
    DEF_REGISTER("&r1",REG_1|REG_IS_ADDRESS);
    DEF_REGISTER("&r2",REG_2|REG_IS_ADDRESS);
    DEF_REGISTER("&r3",REG_3|REG_IS_ADDRESS);
    DEF_REGISTER("&r4",REG_4|REG_IS_ADDRESS);
    DEF_REGISTER("&r5",REG_5|REG_IS_ADDRESS);
    DEF_REGISTER("&r6",REG_6|REG_IS_ADDRESS);
    DEF_REGISTER("&r7",REG_7|REG_IS_ADDRESS);
    DEF_REGISTER("&r8",REG_8|REG_IS_ADDRESS);
    DEF_REGISTER("&r9",REG_9|REG_IS_ADDRESS);
    DEF_REGISTER("&r10",REG_10|REG_IS_ADDRESS);
    DEF_REGISTER("&r11",REG_11|REG_IS_ADDRESS);
    DEF_REGISTER("&r12",REG_12|REG_IS_ADDRESS);
    DEF_REGISTER("&r13",REG_13|REG_IS_ADDRESS);
    DEF_REGISTER("&r14",REG_14|REG_IS_ADDRESS);

    DEF_REGISTER("-",REG_0);
    DEF_REGISTER("C",REG_C);

#undef DEF_REGISTER

    return DataType_IGNORE;
}

const uint16_t MachineCodeAssembler::GetConstantData(const std::string& a_Data)const
{
    uint16_t data;
    sscanf(a_Data.c_str(),"0x%hx",&data);

    return data;
}

Instruction MachineCodeAssembler::MakeInstruction(const std::string& a_InstructionDescription)const
{
    Instruction newInstruction;
    *((u_int32_t*)(&newInstruction)) = 0;

    const StringVec operation = SplitInstruction(a_InstructionDescription);
    const std::string instruction = operation[0];
    const StringVec params = SplitString(operation[1],",");
    
    if( params.size() != 4 )
    {
        throw std::runtime_error("Malformed instruction, all instructions have four params, even if not used. num params found " + std::to_string(params.size()) + " " + a_InstructionDescription);
    }

    newInstruction.DataType = GetDataType(params[0]);

    const uint32_t source = GetRegister(params[1]);
    const uint32_t dest = GetRegister(params[2]);

    if( !IsRegisterConstant(source) )
    {// Get the index, may be register or the register as an address. Hence the mask.
        newInstruction.Source = (source&0x7);
    }

    if( !IsRegisterConstant(source) )
    {// Get the index, may be register or the register as an address. Hence the mask.
        newInstruction.Dest = (dest&0x7);
    }

    newInstruction.ConstantData = GetConstantData(params[3]);

    newInstruction.OpCode = GetOpCode(instruction,source,dest);

    return newInstruction;
}

uint32_t MachineCodeAssembler::GetOpCode(const std::string& a_Instuction,uint32_t a_Source,uint32_t a_Dest)const
{
#define MAKE_OPCODE(__OP_NAME__,__OP_CODE__)  if( CompareNoCase(a_Instuction,(__OP_NAME__)) ){return __OP_CODE__;}
    OPERATION_CODES
#undef MAKE_OPCODE

    throw std::runtime_error("Unknown instruction found " + a_Instuction);

    return 0;
}

