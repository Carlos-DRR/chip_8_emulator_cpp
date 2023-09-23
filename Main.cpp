#include <iostream>
#include "Chip8.h"

int main(){
    std::cout << "Chip 8 Emulator" << std::endl;
    Chip8 *chip = new Chip8();
    system("pause");
    return 0;
}