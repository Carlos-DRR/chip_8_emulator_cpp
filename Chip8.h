#ifndef CHIP8_H
#define CHIP8_H
#include "Stack.h"
#include "Display.h"
#include <cstdint>
#include <iostream>

class Chip8{
    private:
        //memory of 4 KB
        uint8_t ram[4096]{};
        //program counter = points to the instruction in memory
        uint16_t pc = 0x0000;
        //register "i"
        uint16_t iReg = 0x0000;
        //stack pointer
        uint8_t sp = 0x00;
        //registers
        uint8_t registers[16]{};
        //special registers that gets decremented at a rate of 60Hz until it reaches 0
        uint8_t delayTimer = 0x00;
        uint8_t soundTimer = 0x00;
        Stack *stack;

        Display *display;
        //chip8 has a built in font sprite data, and stores it in memory
        //the sprites are numbers from 0 to 16 in hex: 0, 1, 2, .., F
        //each character is represented in 5 bytes
        //example of number 0:
        //11110000
        //10010000
        //10010000
        //10010000
        //11110000
        //the "i" register fetches sprite data
        void initializeFontSprites();
    public:
        Chip8();
        uint16_t getPc();
        //returns true if PC must be incremented
        //as some instructions JUMPS
        bool decodeAndExecute(uint16_t instruction);
        void run();

};

#endif