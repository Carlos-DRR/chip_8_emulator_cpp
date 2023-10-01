#include <iostream>
#include "Chip8.hpp"

int main(int argc, char *argv[] ){
    
    Chip8 *chip = new Chip8("C:\\Users\\carlo\\Desktop\\chip_8_emulator\\roms\\brix.ch8");
    chip->run();
    system("pause");
    return 0;
}