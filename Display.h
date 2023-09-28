
#ifndef DISPLAY_H
#define DISPLAY_H
#include <cstdint>
#include <iostream>
#include <bitset>
#include <SDL2/SDL.h>
#define WIDTH 64
#define HEIGHT 32
#define TEX_WIDTH 64
#define TEX_HEIGHT 32

class Display{
    private:
        bool screen[HEIGHT][WIDTH]{};
        bool setBitsInScreen(uint8_t result, int xPos, int yPos);
        int setPositionInHeightBounds(int xPos);
        int setPositionInWidthBounds(int yPos);
        SDL_Texture *tex;
        SDL_Renderer *renderer;
    public:
        Display(SDL_Renderer *renderer, SDL_Texture *tex);
        void clear();
        bool *getScreen();
        bool setByteInScreen(uint8_t byte, int xPos, int yPos);
        uint8_t getSreenByte(int xPos, int yPos);
        void print();
};

#endif