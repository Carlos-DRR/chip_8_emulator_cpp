#include <iostream>
#include "Chip8.h"
//#include <stdio.h>
//#include "Display.h"

int main(int argc, char *argv[] ){
    
    Chip8 *chip = new Chip8("C:\\Users\\carlo\\Desktop\\chip_8_emulator\\roms\\pong.ch8");
    chip->run();
    system("pause");
    return 0;
}