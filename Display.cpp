#include "Display.h"

Display::Display(){

}
void Display::clear(){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            screen[i][j] == false;
        }
    }
}

bool * Display::getScreen(){
    return screen[0];
}

int Display::setPositionInHeightBounds(int verticalPosition){
    if(verticalPosition >= (2 * (HEIGHT - 1))) return verticalPosition % HEIGHT;
    if(verticalPosition < 0) verticalPosition = HEIGHT + verticalPosition;
    else if(verticalPosition > HEIGHT - 1) verticalPosition = verticalPosition - HEIGHT;
    return verticalPosition;
}

int Display::setPositionInWidthBounds(int horizontalPosition){
    if(horizontalPosition >= (2 * (WIDTH - 1))) return horizontalPosition % WIDTH;
    if(horizontalPosition < 0) horizontalPosition = WIDTH + horizontalPosition;
    else if(horizontalPosition > WIDTH - 1) horizontalPosition = horizontalPosition - WIDTH;
    return horizontalPosition;
}

uint8_t Display::getSreenByte(int verticalPosition, int horizontalPosition){
    uint8_t result = 0x00;
    int shiftAmount = 7;
    uint8_t tempValue;
    for(int i = 0; i < 8; i++){
        if(screen[verticalPosition][horizontalPosition] == true){
            tempValue = 0b00000001;
            tempValue = tempValue << shiftAmount;
            result += tempValue;
            
            
        }else{
            tempValue = 0b00000000;
            tempValue = tempValue << shiftAmount;
            result += tempValue;
        }
        shiftAmount--;
        horizontalPosition += 1;
        horizontalPosition = setPositionInWidthBounds(horizontalPosition);
    }
    return result;
}

//returns true if any bit was inverted
bool Display::setBitsInScreen(uint8_t newScreenByte, int verticalPosition, int horizontalPosition){
    uint8_t mask = 0b10000000;
    uint8_t tempResult;
    int inverted = 0;
    for(int i = 0; i < 8; i++){
        tempResult = mask & newScreenByte;
        if(tempResult > 0){
            //screen[verticalPosition][horizontalPosition] == false ? inverted++ : 0;
            screen[verticalPosition][horizontalPosition] = true;
        } 
        else{
            screen[verticalPosition][horizontalPosition] == true ? inverted+=1 : inverted+=0;
            screen[verticalPosition][horizontalPosition] = false;
        } 
        horizontalPosition++;
        mask = mask >> 1;
        horizontalPosition = setPositionInWidthBounds(horizontalPosition);
    }
    return inverted > 0;
}

bool Display::setByteInScreen(uint8_t byte, int horizontalPosition, int verticalPosition){
    uint8_t currentScreenByte;
    bool inverted;
    verticalPosition = setPositionInHeightBounds(verticalPosition);
    horizontalPosition = setPositionInWidthBounds(horizontalPosition);
    currentScreenByte = getSreenByte(verticalPosition, horizontalPosition);
    uint8_t newScreenByte = currentScreenByte ^ byte;
    inverted = setBitsInScreen(newScreenByte, verticalPosition, horizontalPosition);
    return inverted;
}


void Display::print(){
    system("cls");
    std::cout << "\n";
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            std::cout << ((screen[i][j] == true) ? 'O' : '.') << " ";
        }
        std::cout << "\n";
    }

}