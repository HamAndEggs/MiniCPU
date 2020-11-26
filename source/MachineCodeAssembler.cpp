#include <iostream>
#include  <iomanip>

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

    int LineNumber = 0;
    const StringVec CodeLines = SplitString(pAssembler,"\n");
    for(const auto &line : CodeLines )
    {
        try
        {
            LineNumber++;
            if( line.size() > 1 )
            {
                const Instruction ins = MakeInstruction(line);
                machineCode.push_back(ins);

                const std::string cleaned = TrimWhiteSpace(SplitString(line,"//")[0]);
                std::cout << "[" << cleaned << "] ";

                if( ins.Standard.IsLoad )
                {
                    std::cout   << ins.Load.IsLoad << " "
                                << ins.Load.OrWithDest << " "
                                << ins.Load.Shift << " "
                                << ins.Load.Dest << " "
                                << ins.Load.ConstantData;
                }
                else if( ins.Standard.OpCode == OP_JUMP )
                {
                    std::cout   << ins.Jump.IsLoad << " "
                                << ins.Jump.OpCode << " "
                                << ins.Jump.Condition << " "
                                << ins.Jump.PCRelative << " "
                                << ins.Jump.OffsetRegister << " "
                                << ins.Jump.ConstantData;
                }
                else
                {
                    std::cout   << ins.Standard.IsLoad << " "
                                << ins.Standard.OpCode << " "
                                << ins.Standard.Source << " "
                                << ins.Standard.SourceIsAddress << " "
                                << ins.Standard.Dest << " "
                                << ins.Standard.DestIsAddress << " "
                                << ins.Standard.DataType << " "
                                << ins.Standard.ConstantData;
                }

                std::cout << " Bytes: 0x" << std::setfill('0') << std::setw(8) << std::hex << ins.Bytes << std::dec;
                std::cout << std::endl;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << LineNumber << ": " << e.what() << " : " << line << std::endl;
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
    DEF_REGISTER("r15",REG_14);

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
    DEF_REGISTER("&r15",REG_15|REG_IS_ADDRESS);

#undef DEF_REGISTER

    return DataType_IGNORE;
}

uint32_t MachineCodeAssembler::GetCondition(const std::string& a_Condition)const
{
    if( TrimWhiteSpace(a_Condition).size() == 0 )
    {
        throw std::runtime_error("Empty condition code not supported");
    }


#define DEF_CONDITION_CODE(__name__,__value__)   if( CompareNoCase(a_Condition,(__name__)) ){return (__value__);}

    DEF_CONDITION_CODE("FALSE",ConCode_FALSE);
    DEF_CONDITION_CODE("TRUE",ConCode_TRUE);
    DEF_CONDITION_CODE("NEQ",ConCode_NEQ);
    DEF_CONDITION_CODE("POS",ConCode_POS);
    DEF_CONDITION_CODE("NZ",ConCode_NZ);
    DEF_CONDITION_CODE("EQ",ConCode_EQ);
    DEF_CONDITION_CODE("NE",ConCode_NE);
    DEF_CONDITION_CODE("LT",ConCode_LT);
    DEF_CONDITION_CODE("GT",ConCode_GT);
    DEF_CONDITION_CODE("LE",ConCode_LE);
    DEF_CONDITION_CODE("GE",ConCode_GE);

#undef DEF_CONDITION_CODE

    throw std::runtime_error("Unknown condition code " + a_Condition);
}

uint16_t MachineCodeAssembler::GetConstantDataUNSIGNED(const std::string& a_Data)const
{
    uint16_t data;
    sscanf(a_Data.c_str(),"0x%hx",&data);

    return data;
}

int16_t MachineCodeAssembler::GetConstantDataSIGNED(const std::string& a_Data)const
{
    int16_t data;
    sscanf(a_Data.c_str(),"0x%hx",&data);

    return data;
}

uint32_t MachineCodeAssembler::GetValue(const std::string& a_Data,uint32_t a_AllowedMax)const
{
    const uint32_t value = std::stoul(a_Data,nullptr,16);
    if( value > a_AllowedMax )
    {
        throw std::runtime_error("Reading value from string failed, allowed range is 0 to " + std::to_string(a_AllowedMax) + " value read was " + std::to_string(value) );
    }

    return value;
}


Instruction MachineCodeAssembler::MakeInstruction(const std::string& a_InstructionDescription)const
{
    Instruction newInstruction;
    newInstruction.Bytes = 0;

    const StringVec operation = SplitInstruction(a_InstructionDescription);
    const std::string instruction = operation[0];
    const StringVec params = SplitString(operation[1],",");
    
    if( params.size() != 4 )
    {
        throw std::runtime_error("Malformed instruction, all instructions have four params, even if not used. num params found " + std::to_string(params.size()) + " " + a_InstructionDescription);
    }

    // We have three command formats.
    // LOAD is denoted by the first bit, and so is not listed in the opcode enum.
    // JUMP is in the opcode but has a different definition of the four params.
    // The rest follow the same rules.
    if( CompareNoCase(instruction,"LOAD") )
    {
        const uint32_t dest = GetRegister(params[2]);
        if( IsRegisterAddress(dest) )
        {
            // Sorry, for jump register can't be a read address.
            throw std::runtime_error("Malformed instruction, the LOAD instruction can not use registers as an indirect address " + a_InstructionDescription);
        }

        newInstruction.Load.IsLoad = 1;
        newInstruction.Load.OrWithDest = GetValue(params[0],1);
        newInstruction.Load.Shift = GetValue(params[1],2);
        newInstruction.Load.Dest = (dest&0x15);

        const uint32_t value = std::stoul(params[3],nullptr,16);

        if( value > 0x00ffffff )
        {
            throw std::runtime_error("Malformed instruction, the constant data for LOAD is too large, only 24bit values allowed. Was given " + params[3]);
        }

        newInstruction.Load.ConstantData = value;

    }
    else if( CompareNoCase(instruction,"JUMP") )
    {
        const uint32_t dest = GetRegister(params[2]);
        if( IsRegisterAddress(dest) )
        {
            // Sorry, for jump register can't be a read address.
            throw std::runtime_error("Malformed instruction, the JUMP instruction can not use registers as an indirect address " + a_InstructionDescription);
        }

        newInstruction.Jump.IsLoad = 0;
        newInstruction.Jump.OpCode = OP_JUMP;
        newInstruction.Jump.Condition = GetCondition(params[0]);
        newInstruction.Jump.PCRelative = GetValue(params[1],1);
        newInstruction.Jump.OffsetRegister = (dest&0x15);
        newInstruction.Jump.ConstantData = GetConstantDataSIGNED(params[3]);

    }
    else
    {
        const uint32_t source = GetRegister(params[1]);
        const uint32_t dest = GetRegister(params[2]);

        newInstruction.Standard.OpCode = GetOpCode(instruction);
        newInstruction.Standard.Source = (source&0x15);
        newInstruction.Standard.Dest = (dest&0x15);

        if( IsRegisterAddress(source) )
        {
            newInstruction.Standard.SourceIsAddress = 1;
        }

        if( IsRegisterAddress(source) )
        {
            newInstruction.Standard.DestIsAddress = 1;
        }

        newInstruction.Standard.DataType = GetDataType(params[0]);
        newInstruction.Standard.ConstantData = GetConstantDataUNSIGNED(params[3]);
    }

    return newInstruction;
}

uint32_t MachineCodeAssembler::GetOpCode(const std::string& a_Instuction)const
{
#define MAKE_OPCODE(__OP_NAME__,__OP_CODE__)  if( CompareNoCase(a_Instuction,(__OP_NAME__)) ){return __OP_CODE__;}
    OPERATION_CODES
#undef MAKE_OPCODE

    throw std::runtime_error("Unknown instruction found " + a_Instuction);

    return 0;
}

