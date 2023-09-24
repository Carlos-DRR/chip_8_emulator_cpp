#include "Display.h"


Display::Display(){

}
bool * Display::getScreen(){
    return *screen;
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
//ASSUMING width and height will not be bigger than 2x width and height
bool Display::setByteInScreen(uint8_t byte, int xPos, int yPos){
    uint8_t byteInScreen;
    bool inverted;

    
    /*
    if(xPos <= WIDTH && xPos >= 0 && yPos <= HEIGHT && yPos >= 0){
        byteInScreen = getSreenByte(xPos, yPos);
        uint8_t result = byteInScreen ^ byte;
        inverted = setBitsInScreen(result, xPos, yPos);
    }
    
    if(yPos < 0) yPos = WIDTH + yPos;
    else if(yPos > WIDTH) yPos = yPos - WIDTH;
    else if(xPos < 0) xPos = HEIGHT + xPos;
    else if(xPos > WIDTH) xPos = xPos - HEIGHT;
    */
    xPos = setPositionInHeightBounds(xPos);
    yPos = setPositionInWidthBounds(yPos);
    std::cout << "xPos initial: " << xPos;
    std::cout << "yPos initial: " << yPos;
    byteInScreen = getSreenByte(xPos, yPos);
    uint8_t result = byteInScreen ^ byte;
    std::cout << "result: ";
    std::cout << std::bitset<8>(result) << std::endl;
    inverted = setBitsInScreen(result, xPos, yPos);
    return inverted;
}


void Display::print(){
    /* teste */
    bool value = true;
    
    for(int j = 189; j < 197; j++){
        std::cout << "value: " << value <<std::endl;
        
        screen[setPositionInHeightBounds(93)][setPositionInWidthBounds(j)] = value;
        value = !value;
    }
    
    /* teste */
    setByteInScreen(0b11111111, 93, 189);
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            std::cout << ((screen[i][j] == true) ? 1 : 0) << " ";
        }
        std::cout << "\n";
    }

}