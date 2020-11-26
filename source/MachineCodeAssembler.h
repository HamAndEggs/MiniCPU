#ifndef __MACINE_CODE_ASSEMBLER_H__
#define __MACINE_CODE_ASSEMBLER_H__

#include <string>
#include <vector>
#include <assert.h>

#include "MiniCPU.h"

/**
 * @brief This compiles an assember source file into machine code. This is NOT a macro assembler.
 * 
 */
class MachineCodeAssembler
{
public:
    MachineCodeAssembler();
    ~MachineCodeAssembler();


    std::vector<Instruction> Compile(const std::string& pAssembler)const;

private:

    uint32_t GetDataType(const std::string& a_Type)const;
    uint32_t GetRegister(const std::string& a_Type)const;
    uint32_t GetCondition(const std::string& a_Condition)const;
    uint16_t GetConstantDataUNSIGNED(const std::string& a_Data)const;
    int16_t GetConstantDataSIGNED(const std::string& a_Data)const;
    uint32_t GetValue(const std::string& a_Data,uint32_t a_AllowedMax)const;
    Instruction MakeInstruction(const std::string& a_InstructionDescription)const;

    bool IsRegisterAddress(uint32_t a_Register)const{return (a_Register&REG_IS_ADDRESS)?true:false;}
    uint32_t GetOpCode(const std::string& a_Instuction)const;


};

#endif //__MACINE_CODE_ASSEMBLER_H__