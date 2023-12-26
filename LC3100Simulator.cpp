/* instruction-level simulator for LC3100 */
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cstring>
#include <math.h>


#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

const int MAX_BLOCKS = 256;

int size = 0;
int address = 0;


enum actionType{
    
    cacheToProcessor, processorToCache, memoryToCache, cacheToMemory,
    cacheToNowhere

};
void printAction(int address, int size, enum actionType type);
    //define stateStruct

    typedef struct stateStruct {
        int pc;
        int mem[NUMMEMORY];
        int reg[NUMREGS];
        int numMemory;
    }   stateType;




//function to convert binary string of the register to decimal

int convertLargeBin(const std::string& binaryString) {
    // Check if the binary string is not empty
    if (binaryString.empty()) {
        std::cerr << "Error: Empty binary string." << std::endl;
        return 0;
    }

    // Get the length of the binary string
    size_t length = binaryString.length();

    // Convert binary string to an integer
    int result = 0;
    for (size_t i = 0; i < length; ++i) {
        if (binaryString[i] == '1') {
            result += (1 << (length - 1 - i));
        }
    }

    // Check the sign bit
    if (binaryString[0] == '1') {
        // It's a negative number, apply 2's complement
        result = result - (1 << length);
    }

    return result;
}

int binToDecimal(char* binaryString){

    if(strcmp(binaryString, "000") == 0){
        return 0;
    } else if(strcmp(binaryString, "001") == 0){
        return 1;
    } else if(strcmp(binaryString, "010") == 0){
        return 2;
    } else if(strcmp(binaryString, "011") == 0){
        return 3;
    } else if(strcmp(binaryString, "100") == 0){
        return 4;
    } else if(strcmp(binaryString, "101") == 0){
        return 5;
    } else if(strcmp(binaryString, "110") == 0){
        return 6;
    } else{
        return 7;
    }

    

}

//function to convert decimal to binary strings

char* intToBinary(int value) {
    static char binaryString[33];  // 32 bits + 1 for null-terminator
    int index = 31;  // Index to fill the binary string from right to left

    binaryString[32] = '\0';  // Null-terminate the string

    if (value < 0) {
        // Handle negative numbers by inverting the bits
        value = ~value + 1;  // Add 1 to get two's complement
        binaryString[0] = '1';  // Set the sign bit to 1 for negative numbers
    } else {
        binaryString[0] = '0';  // Set the sign bit to 0 for non-negative numbers
    }

    // Convert the absolute value to binary
    while (index > 0) {
        binaryString[index] = (value & 1) ? '1' : '0';
        value >>= 1;
        index--;
    }

    return binaryString;
}

   

    void printState(stateType *);


    //main driver
    int j = 15;
    double offSetNum;
    double exponenet = 0;
    
    int main(int argc, char *argv[]){

        

    //string to hold binary instructions
    char workingReg[33];
    
    //string to hold opcode
    char temp[4];
    char opcode[4];
    char offSetString[17];
    int  regA = 0;
    int  regB = 0;
    int  destReg = 0;
    
    
    char line[MAXLINELENGTH];

    //create state struct variable
    stateType state;
    state.pc = 0;
    FILE *filePtr;

    if (argc != 2 && argc != 5) {
    printf("error: usage: %s <machine-code file>\n", argv[0]);
    exit(1);
}

    filePtr = fopen(argv[1], "r");

    if(argc == 5){
    
    
    int blockSize = atoi(argv[2]);
    int numSets = atoi(argv[3]);
    int blocksPerSet = atoi(argv[4]);

    int cache[numSets][blocksPerSet][blockSize]; //[sets][blocks][block-content]
    }
    
    



    if (filePtr == NULL) {
    printf("error: can't open file %s", argv[1]);
    perror("fopen");
    exit(1);
}

/* read in the entire machine-code file into memory */
    
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
        state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
        printf("error in reading address %d\n", state.numMemory);
        exit(1);
    }
    printf("memory[%d]=%d\n", state.numMemory,
state.mem[state.numMemory]);

    
}
   // Initialize regs to all zeros
   state.reg[0] = 0;
   state.reg[1] = 0;
   state.reg[2] = 0;
   state.reg[3] = 0;
   state.reg[4] = 0;
   state.reg[5] = 0;
   state.reg[6] = 0;
   state.reg[7] = 0;

   actionType action = cacheToNowhere;
   
   for(int i = state.pc; i<10000; i++){

    

    

    
    printState(&state);
    
    
    memset(workingReg, '0', 32);
    workingReg[32]='\0';
    //copy binary into working register
    strcpy(workingReg, intToBinary(state.mem[i]));

   
    //get opcode from binary instruction
    opcode[0] = workingReg[7];
    opcode[1] = workingReg[8];
    opcode[2] = workingReg[9];
    
    


    if (opcode[0] == '0' && opcode[1] == '0' && opcode[2] == '0') {
    

    
    //get register A
    temp[0] = workingReg[10];
    temp[1] = workingReg[11];
    temp[2] = workingReg[12];
    temp[3] = '\0';
    
   
    regA = binToDecimal(temp);
    
    
    //get register B

    temp[0] = workingReg[13];
    temp[1] = workingReg[14];
    temp[2] = workingReg[15];
    temp[3] = '\0';

  
    regB = binToDecimal(temp);
    

    //get destination register

    temp[0] = workingReg[29];
    temp[1] = workingReg[30];
    temp[2] = workingReg[31];
    temp[3] = '\0';

    destReg = binToDecimal(temp);
    action = cacheToProcessor;
    printAction(address, size, action);

    state.reg[destReg] = (state.reg[regA]+ state.reg[regB]);
    state.pc++;

} if (opcode[0] == '0' && opcode[1] == '0' && opcode[2] == '1') {
    
    
    //get register A

    temp[0] = workingReg[10];
    temp[1] = workingReg[11];
    temp[2] = workingReg[12];
    temp[3] = '\0';
   
   
    regA = binToDecimal(temp);
   

    //get register B

    temp[0] = workingReg[13];
    temp[1] = workingReg[14];
    temp[2] = workingReg[15];
    temp[3] = '\0';
    

    regB = binToDecimal(temp);
    

    //get destination register

    temp[0] = workingReg[29];
    temp[1] = workingReg[30];
    temp[2] = workingReg[31];
    temp[3] = '\0';

    destReg = binToDecimal(temp);
    
    state.pc++;

} else if (opcode[0] == '0' && opcode[1] == '1' && opcode[2] == '0'){


    
    //get register A

    temp[0] = workingReg[10];
    temp[1] = workingReg[11];
    temp[2] = workingReg[12];
    temp[3] = '\0';
    
   
    regA = binToDecimal(temp);
    
    address = regA;
    //get register B

    temp[0] = workingReg[13];
    temp[1] = workingReg[14];
    temp[2] = workingReg[15];
    temp[3] = '\0';
   

    regB = binToDecimal(temp);
    

    //get offset value

    offSetString[0] = workingReg[16];
    offSetString[1] = workingReg[17];
    offSetString[2] = workingReg[18];
    offSetString[3] = workingReg[19];
    offSetString[4] = workingReg[20];
    offSetString[5] = workingReg[21];
    offSetString[6] = workingReg[22];
    offSetString[7] = workingReg[23];
    offSetString[8] = workingReg[24];
    offSetString[9] = workingReg[25];
    offSetString[10] = workingReg[26];
    offSetString[11] = workingReg[27];
    offSetString[12] = workingReg[28];
    offSetString[13] = workingReg[29];
    offSetString[14] = workingReg[30];
    offSetString[15] = workingReg[31];
    offSetString[16] = '\0';
    
    
    exponenet = 0;
    j = 15;
    offSetNum = 0;
    
    for( j ; j>=0; j--){
        
        if(offSetString[j] == '1'){
            offSetNum+= pow(2.0, exponenet);
        }
        exponenet++;
        
    }

   
   
    state.reg[regB] = (state.mem[(int)offSetNum + (int)state.reg[regA]]);
    
    
    
    action = memoryToCache;
    printAction(address, size, action);
    action = cacheToProcessor;
    printAction(address, size, action);
  

    state.pc++;

    

} else if (opcode[0] == '0' && opcode[1] == '1' && opcode[2] == '1') {
    
    

    //get register A

    temp[0] = workingReg[10];
    temp[1] = workingReg[11];
    temp[2] = workingReg[12];
    temp[3] = '\0';
   
   
    regA = binToDecimal(temp);
    
    address = regA;

    //get register B

    temp[0] = workingReg[13];
    temp[1] = workingReg[14];
    temp[2] = workingReg[15];
    temp[3] = '\0';
    

    regB = binToDecimal(temp);
   

    //get offset value
    
    
    memcpy(offSetString, &workingReg[16], 16);
    offSetString[16] = '\0';

    action = processorToCache;
    printAction(address, size, action);
    action = cacheToMemory;
    printAction(address, size, action);

    state.pc++;

} else if (opcode[0] == '1' && opcode[1] == '0' && opcode[2] == '0') {
    

    //get register A

    temp[0] = workingReg[10];
    temp[1] = workingReg[11];
    temp[2] = workingReg[12];
    temp[3] = '\0';

   
  
   

    regA = state.reg[binToDecimal(temp)];
    
    //get register B

    temp[0] = workingReg[13];
    temp[1] = workingReg[14];
    temp[2] = workingReg[15];
    temp[3] = '\0';
    
    regB = state.reg[binToDecimal(temp)];

    //get offset

    offSetString[0] = workingReg[16];
    offSetString[1] = workingReg[17];
    offSetString[2] = workingReg[18];
    offSetString[3] = workingReg[19];
    offSetString[4] = workingReg[20];
    offSetString[5] = workingReg[21];
    offSetString[6] = workingReg[22];
    offSetString[7] = workingReg[23];
    offSetString[8] = workingReg[24];
    offSetString[9] = workingReg[25];
    offSetString[10] = workingReg[26];
    offSetString[11] = workingReg[27];
    offSetString[12] = workingReg[28];
    offSetString[13] = workingReg[29];
    offSetString[14] = workingReg[30];
    offSetString[15] = workingReg[31];
    offSetString[16] = '\0';
 
    int offset = convertLargeBin(offSetString);


    if(regA == regB){
        state.pc = state.pc+1+offset;
        i = state.pc;
    }

    else{
        state.pc++;
    }

    

    
} else if (opcode[0] == '1' && opcode[1] == '0' && opcode[2] == '1') {
   
    state.pc++;
} else if (opcode[0] == '1' && opcode[1] == '1' && opcode[2] == '0') {
    
    state.pc++;
    std::cout<<"MACHINE HALTED"<<std::endl;
    std::cout<<"FINAL STATE OF MACHINE:"<<std::endl<<std::endl;
    break;
} else if (opcode[0] == '1' && opcode[1] == '1' && opcode[2] == '1') {
    state.pc++;
    
    
}

 

   }
    
    
printState(&state);

return(0);
}

void printAction(int address, int size, enum actionType type)
{
    printf("@@@ transferring word [%d-%d] ", address, address + size - 1);
    if (type == cacheToProcessor) {
        printf("from the cache to the processor\n");
    } else if (type == processorToCache) {
        printf("from the processor to the cache\n");
    } else if (type == memoryToCache) {
        printf("from the memory to the cache\n");
    } else if (type == cacheToMemory) {
        printf("from the cache to the memory\n");
    } else if (type == cacheToNowhere) {
        printf("from the cache to nowhere\n");
    }
}

void printState(stateType *statePtr){

    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");

    for (i=0; i<statePtr->numMemory; i++) {

    printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);

    }

printf("\tregisters:\n");

for (i=0; i<NUMREGS; i++) {

printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);

}

printf("end state\n");
}
