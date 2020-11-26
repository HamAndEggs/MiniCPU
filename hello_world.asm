LOAD 0,0,R2,0x012345         // Set R2 to a 64bit address
LOAD 1,1,R2,0x6789AB
LOAD 1,2,R2,0x00CDEF
LOAD 0,0,R7,0x111122         // Set R7 to a 64bit value
LOAD 1,1,R7,0x223333
LOAD 1,2,R7,0x004444
MOVE S32,$R2,R1,0x0000       // Get value at address R2
MOVE S32,$R2,R3,0x0004      // Get value at address R2 + 4
ADD  S32,R1,R3,0x0000        // Add both together.
MOVE S32,R3,&R2,0x0008      // Write result to address R2 + 8 byte offset.
MOVE S32,R15,R0,0x00ff         // Lets do a loop.... 255 times.
MOVE S32,R15,R1,0x0000         // We'll accumulate in here.
ADD  S32,$R2,R1,0x0000       // Add what is at address in R2 to R1
ADD  S64,R15,R2,0x0004        // Increment R2 address by 4 bytes.
SUB  S32,R15,R0,0x0001         // Decrement counter
JUMP NZ,1,R15,0xfffd         // Jump back 3 instructions from current PC. 0xfffd == -3