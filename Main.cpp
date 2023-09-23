#include <iostream>
#include "Chip8.h"
#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 32
#define SCALE_FACTOR 20

int main(int argc, char *argv[] ){
    std::cout << "Chip 8 Emulator" << std::endl;
    Chip8 *chip = new Chip8();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Event event;
    SDL_Window *window = SDL_CreateWindow("Chip 8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH * SCALE_FACTOR, WINDOW_HEIGHT * SCALE_FACTOR, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
    SDL_MaximizeWindow(window);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, 400, 300);
    SDL_RenderPresent(renderer);
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    system("pause");
    return 0;
}