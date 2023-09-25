#include "Chip8.h"



Chip8::Chip8(std::string programPath){
    //addresses 0x000 to 0x1FF are reserved
    //programs are written after 0x1FF
    //so program counter starts at 0x200 (512)
    pc = 0x0200;
    /*
    //test
    //sprite data
    ram[0x0220] = 0xBA;
    ram[0x0221] = 0x7C;
    ram[0x0222] = 0xD6;
    ram[0x0223] = 0xFE;
    ram[0x0224] = 0x54;
    ram[0x0225] = 0xAA;
    //Vx = 10 and Vy = 12
    ram[0x0200] = 0x62;
    ram[0x0201] = 0x0A;
    ram[0x0202] = 0x63;
    ram[0x0203] = 0x0C;

    //load i with address: 220 
    ram[0x0204] = 0xA2;
    ram[0x0205] = 0x20;

    //Draw whatever is stored in ram starting from iReg(0x0220) address to iReg + 6 (height);
    ram[0x0206] = 0xD2;
    ram[0x0207] = 0x36;
    */




    stack = new Stack();
    display = new Display();
    initializeFontSprites();
    readProgram(programPath);
}

uint16_t Chip8::getPc(){
    return unsigned(pc);
}

void Chip8::readProgram(std::string programPath){
    std::ifstream fin;
    fin.open(programPath, std::ios::in | std::ios::binary);
    uint8_t byteData = 0x00;
    uint16_t adress = pc;

    if(fin){
        while(!fin.eof()){
            if(adress > 4096) break;//error not enough memory
            fin.read(reinterpret_cast<char*>(&byteData), 1);
            ram[adress] = byteData;
            adress++;
           
            //std::cout << unsigned(mstSigByte) << std::hex << std::endl;
            //std::cout << unsigned(lstSigByte) << std::hex << std::endl;
        }
        fin.close();
    }else{
        std::cout << "Error opening program file in: " << programPath << std::endl;
    }
}

void Chip8::initializeFontSprites(){
    int spriteByteIndex = 0;
    uint8_t spriteData[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    //most people stores font sprite data between 0x50 and 0x9F in RAM
    for(int i = 0x50; i < 0xA0; i++){
        ram[i] = spriteData[spriteByteIndex];
        spriteByteIndex++;
    }
}

uint16_t getOperation(uint16_t instruction, uint16_t mask){
    return instruction & mask;
}

bool Chip8::decodeAndExecute(uint16_t instruction){
    //instructions are divided by the first half-byte
    uint16_t operation = getOperation(instruction, 0xF000);
    switch(operation){
        case 0x0000:{
            uint16_t operation = getOperation(instruction, 0x000F);
            if(operation == 0x0000){//0x00E0
                //clear the screen
                display->clear();
            }else{//0x00EE
                //return from subroutine
                uint16_t retAddress = stack->pop();
                pc = retAddress;
            }
            break;
        }
        case 0x1000:{
            //Absolute JUMP, example: JUMP $2A8 (0x12A8)
            //jump to address NNN
            uint16_t address = getOperation(instruction, 0x0FFF);
            pc = 0x0000;
            pc += address;
            return false;
            break;
        }
        case 0x2000:{
            //JUMP to a subroutine at address NNN
            uint16_t subRoutineAddress = getOperation(instruction, 0x0FFF); 
            stack->push(pc);
            pc = subRoutineAddress;
            return false;
            break;
        }
        case 0x3000:{
            std::cout << "0x3000 not implemented: " << std::endl; 
            break;
        }
        case 0x4000:{
            std::cout << "0x4000 not implemented: " << std::endl; 
            break;
        }
        case 0x5000:{
            std::cout << "0x5000 not implemented: " << std::endl; 
            break;
        }
        case 0x6000:{
            //Set value NN to register Vx
            uint16_t reg = getOperation(instruction, 0x0F00);
            reg = reg >> 8;
            uint16_t constant = getOperation(instruction, 0x00FF);
            registers[reg] = constant;
            break;
        }
        case 0x7000:{
            //Add value NN to register Vx
            uint16_t reg = getOperation(instruction, 0x0F00);
            reg = reg >> 8;
            uint16_t constant = getOperation(instruction, 0x00FF);
            registers[reg] += constant;
            break;
        }
        case 0x8000:{
            std::cout << "0x8000 not implemented: " << std::endl; 
            break;
        }
        case 0x9000:{
            std::cout << "0x9000 not implemented: " << std::endl; 
            break;
        }
        case 0xA000:{
            //Load register "i" with constant value NNN
            uint16_t value = getOperation(instruction, 0x0FFF);
            iReg = value;
            break;
        }
        case 0xB000:{
            std::cout << "0xB000 not implemented: " << std::endl;
            break;
        }
        case 0xC000:{
            std::cout << "0xC000 not implemented: " << std::endl;
            break;
        }
        case 0xD000:{
            //DRAW sprite Vx, Vy, N = Height;
            //byte posx posy
            uint16_t iRegStartValue = iReg;
            uint16_t regX = getOperation(instruction, 0x0F00);
            regX = regX >> 8;
            uint16_t regY = getOperation(instruction, 0x00F0);
            regY = regY >> 4;
            uint16_t height = getOperation(instruction, 0x000F);
            int xPos = registers[regX];
            int yPos = registers[regY];
            registers[15] = 0;

            for(int i = 0; i < height; i++){
                bool inverted = display->setByteInScreen(ram[iRegStartValue], xPos, yPos);
                //std::cout << "Collision (bit got inverted):" << inverted << std::endl;
                if(inverted == true) registers[15] = 1;
                yPos += 1;
                iRegStartValue += 1;
            }
            std::cout << "\n";
            display->print();
            break;
        }
        case 0xE000:{
            std::cout << "0xE000 not implemented: " << std::endl;
            break;
        }
        case 0xF000:{
            std::cout << "0xF000 not implemented: " << std::endl;
            break;
        }
    }
    return true;
}
void Chip8::run(){
    uint16_t instruction;

    //int i = 66;//debug
    while(1){//debug
        //std::cout << "Current PC value: " << std::hex << pc;
        //std::cout << "\n";
        //fetch with big endian convention
        //each instruction is 2 bytes, the first byte is stored most-significant-byte first
        instruction = ram[pc] << 8;
        instruction += ram[pc+1];
        //decode and execute
        bool canIncrementPc = decodeAndExecute(instruction);
        //some instructions (like JUMPS) don't need to increment PC immediatly after jumping
        (canIncrementPc == true) ?  pc += 2 : pc += 0;
        //i--;//debug
    }
}