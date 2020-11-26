
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <array>

#include "Util.h"
#include "MiniCPU.h"
#include "MachineCodeAssembler.h"


static std::string ReadTextFile(const std::string& a_Filename)
{
    std::ifstream in(a_Filename);
    std::stringstream buf;
    buf << in.rdbuf();
    return buf.str();
}

int main(int argc, char *argv[])
{
// Say hello to the world!
    std::cout << "MiniCPU emulator playground..." << std::endl;

    std::cout << "sizeof Register = " << sizeof(Register) << std::endl;
    std::cout << "sizeof StandardInstruction = " << sizeof(StandardInstruction) << std::endl;
    std::cout << "sizeof LoadInstruction = " << sizeof(LoadInstruction) << std::endl;
    std::cout << "sizeof JumpInstruction = " << sizeof(JumpInstruction) << std::endl;
    std::cout << "sizeof Instruction = " << sizeof(Instruction) << std::endl;

    std::cout << "NUMBER_OPERATIONS = " << NUMBER_OPERATIONS << std::endl;
    std::cout << "OP_LAST = " << OP_LAST << std::endl;
    std::cout << "NUMBER_REGISTERS = " << NUMBER_REGISTERS << std::endl;
    

    const std::string code = ReadTextFile("./hello_world.asm");

    MachineCodeAssembler assembler;

    const std::vector<Instruction> machineCode = assembler.Compile(code);

// And quit
    return 0;
}

