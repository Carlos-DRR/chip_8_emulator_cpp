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

int Display::setPositionInHeightBounds(int xPos){
    if(xPos >= (2 * (HEIGHT - 1))) return xPos % HEIGHT;
    if(xPos < 0) xPos = HEIGHT + xPos;
    else if(xPos > HEIGHT - 1) xPos = xPos - HEIGHT;
    return xPos;
}

int Display::setPositionInWidthBounds(int yPos){
    if(yPos >= (2 * (WIDTH - 1))) return yPos % WIDTH;
    if(yPos < 0) yPos = WIDTH + yPos;
    else if(yPos > WIDTH - 1) yPos = yPos - WIDTH;
    return yPos;
}

uint8_t Display::getSreenByte(int xPos, int yPos){
    uint8_t result = 0x00;
    int shiftAmount = 7;
    uint8_t tempValue;
    for(int i = 0; i < 8; i++){
        if(screen[xPos][yPos] == true){
            tempValue = 0b00000001;
            tempValue = tempValue << shiftAmount;
            result += tempValue;
            
            
        }else{
            tempValue = 0b00000000;
            tempValue = tempValue << shiftAmount;
            result += tempValue;
        }
        shiftAmount--;
        yPos += 1;
        yPos = setPositionInWidthBounds(yPos);
    }
    return result;
}

//returns true if any bit was inverted
bool Display::setBitsInScreen(uint8_t result, int xPos, int yPos){
    uint8_t mask = 0b10000000;
    uint8_t tempResult;
    int shiftAmount = 1;
    int inverted = 0;
    for(int i = 0; i < 8; i++){
        tempResult = mask & result;
        if(tempResult != mask & result) inverted += 1;
        if(tempResult > 0) screen[xPos][yPos] = true;
        else screen[xPos][yPos] = false;
        yPos++;
        mask = mask >> shiftAmount;
        yPos = setPositionInWidthBounds(yPos);
    }

    return inverted > 0;
}

bool Display::setByteInScreen(uint8_t byte, int xPos, int yPos){
    uint8_t byteInScreen;
    bool inverted;
    xPos = setPositionInHeightBounds(xPos);
    yPos = setPositionInWidthBounds(yPos);
    byteInScreen = getSreenByte(xPos, yPos);
    uint8_t result = byteInScreen ^ byte;
    inverted = setBitsInScreen(result, xPos, yPos);
    return inverted;
}


void Display::print(){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            std::cout << ((screen[i][j] == true) ? '*' : ' ') << " ";
        }
        std::cout << "\n";
    }

}