#include "Chip8.hpp"

void audio_callback(void *userdata, uint8_t *stream, int len) {
    uint32_t square_wave_freq = 440; 
    uint32_t audio_sample_rate = 44100; 
    int16_t volume = 3000;            

    int16_t *audio_data = (int16_t *)stream;
    static uint32_t running_sample_index = 0;
    const int32_t square_wave_period = audio_sample_rate / square_wave_freq;
    const int32_t half_square_wave_period = square_wave_period / 2;

    for (int i = 0; i < len / 2; i++)
        audio_data[i] = ((running_sample_index++ / half_square_wave_period) % 2) ? 
                        volume : 
                        -volume;
}
Chip8::Chip8(std::string programPath){
    //addresses 0x000 to 0x1FF are reserved
    //programs are written after 0x1FF
    //so program counter starts at 0x200 (512)
    srand(time(NULL));
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

    //Subroutine stating at 0x0400 that loads 10 to register V0 and adds 15 to it
    //than it returns
    /*
    ram[0x0400] = 0x60;
    ram[0x0401] = 0x10;
    ram[0x0402] = 0x70;
    ram[0x0403] = 0x15;
    ram[0x0404] = 0x00;
    ram[0x0405] = 0xEE;
    */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    window = SDL_CreateWindow("Chip 8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    tex = SDL_CreateTexture(renderer, 
                            SDL_PIXELFORMAT_ARGB8888, 
                            SDL_TEXTUREACCESS_STREAMING,
                            TEX_WIDTH,
                            TEX_HEIGHT);

    want.freq = 44100;
    want.format = AUDIO_S16LSB;          
    want.channels = 1;  
    want.samples = 512;           
    want.callback = audio_callback;
    dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if(dev == 0) SDL_Log("Could not get an audio device %s\n", SDL_GetError());
    if ((want.format != have.format) ||
        (want.channels != have.channels)) {
        SDL_Log("Could not get desired Audio Spec\n");
    }

    stack = new Stack();
    display = new Display(renderer, tex);
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
    //0x50 + (0x00 * 5) = 0x00
    //0x50 + (0x01 * 5) = 0x05
    //0x50 + (0x02 * 5) = 0x0A
    //iReg = memAddStart + (charHex * 5)
    //most people stores font sprite data between 0x50 and 0x9F in RAM
    for(int i = 0x050; i <= 0x09F; i++){
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
            //3XNN v[x] == NN ? (skip next instruction: dont skip)
            uint16_t reg = getOperation(instruction, 0x0F00);
            reg = reg >> 8;
            uint16_t constant = getOperation(instruction, 0x00FF);
            if(registers[reg] == constant) pc += 2;
            break;
        }
        case 0x4000:{
            //4XNN v[x] != NN ? (skip next instruction: dont skip)
            uint16_t reg = getOperation(instruction, 0x0F00);
            reg = reg >> 8;
            uint16_t constant = getOperation(instruction, 0x00FF);
            if(registers[reg] != constant) pc += 2;
            break;
        }
        case 0x5000:{
            uint16_t regX = getOperation(instruction, 0x0F00);
            regX = regX >> 8;
            uint16_t regY = getOperation(instruction, 0x00F0);
            regY = regY >> 4;
            if(registers[regX] == registers[regY]) pc += 2;
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
            //HERE
            uint16_t regX = getOperation(instruction, 0x0F00);
            regX = regX >> 8;
            uint16_t regY = getOperation(instruction, 0x00F0);
            regY = regY >> 4;
            uint16_t op = getOperation(instruction, 0x000F);

            switch(op){
                case 0x0000:{
                    registers[regX] = registers[regY];
                    break;
                }

                case 0x0001:{
                    registers[regX] = registers[regX] | registers[regY];
                    break;
                }

                case 0x0002:{
                    registers[regX] = registers[regX] & registers[regY];
                    break;
                }

                case 0x0003:{
                    registers[regX] = registers[regX] ^ registers[regY];
                    break;
                }

                case 0x0004:{
                    uint16_t result = registers[regX] + registers[regY];
                    registers[regX] = result;
                    if(result > 255) registers[15] = 1;
                    else registers[15] = 0;
                    break;
                }

                case 0x0005:{
                    uint8_t x = registers[regX];
                    uint8_t y = registers[regY];                    
                    registers[regX] -= registers[regY];
                    if(x > y) registers[15] = 1;
                    else registers[15] = 0;
                    break;
                }
                case 0x0006:{
                    uint8_t mask = 0b00000001;
                    uint8_t shiftedOutBit = registers[regX] & mask;
                    registers[regX] = registers[regX] >> 1;
                    registers[15] = shiftedOutBit;
                    
                    break;
                }
                case 0x0007:{                   
                    uint8_t x = registers[regX];
                    uint8_t y = registers[regY];
                    registers[regX] = registers[regY] - registers[regX];
                    if(y  > x) registers[15] = 1;
                    else registers[15] = 0;
                    break;
                }
                case 0x000E:{//left
                    uint8_t shiftedOutBit = registers[regX] >> 7;
                    registers[regX] = registers[regX] << 1;
                    registers[15] = shiftedOutBit;
                    break;
                }
            }
            break;
        }
        case 0x9000:{
            uint16_t regX = getOperation(instruction, 0x0F00);
            regX = regX >> 8;
            uint8_t regY = getOperation(instruction, 0x00F0);
            regY = regY >> 4;
            if(registers[regX] != registers[regY])pc += 2;
            return true;
            break;
        }
        case 0xA000:{
            //Load register "i" with constant value NNN
            uint16_t value = getOperation(instruction, 0x0FFF);
            iReg = value;
            break;
        }
        case 0xB000:{//jumps to address NNN plus the value in V0
            uint16_t address = getOperation(instruction, 0x0FFF);
            pc = registers[0];
            pc += address;
            return false;
            break;
        }
        case 0xC000:{
            uint16_t regX = getOperation(instruction, 0x0F00);
            regX = regX >> 8;
            uint8_t constant = getOperation(instruction, 0x00FF);
            uint8_t randNumber = (rand() % 256);
            registers[regX] = randNumber & constant;
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
                if(inverted == true) registers[15] = 1; //change this
                yPos += 1;
                iRegStartValue += 1;
            }
            display->print();
            break;
        }
        case 0xE000:{
            uint16_t op = getOperation(instruction, 0x00FF);
            switch(op){
                case 0x009E:{
                    uint16_t reg = getOperation(instruction, 0x0F00);
                    reg = reg >> 8;
                    uint8_t value = registers[reg];
                    if(value == getKey()) pc += 2;
                    break;
                }
                case 0x00A1:{
                    uint16_t reg = getOperation(instruction, 0x0F00);
                    reg = reg >> 8;
                    uint8_t value = registers[reg];
                    if(value != getKey()) pc += 2;
                    break;
                }
                
            }
            break;
        }
        case 0xF000:{
            uint16_t op = getOperation(instruction, 0x00FF);
            //std::cout << "0xF000 not completely implemented: " << std::endl;
            switch(op){
                case 0x0007:{
                    uint16_t reg = getOperation(instruction, 0x0F00);
                    reg = reg >> 8;
                    registers[reg] = delayTimer;
                    break;
                }
                case 0x0015:{
                    uint16_t reg = getOperation(instruction, 0x0F00);
                    reg = reg >> 8;
                    delayTimer = registers[reg];
                    break;
                }
                case 0x0018:{
                    uint16_t reg = getOperation(instruction, 0x0F00);
                    reg = reg >> 8;
                    soundTimer = registers[reg];   
                    break;
                }
                case 0x001E:{
                    uint16_t reg = getOperation(instruction, 0x0F00);
                    reg = reg >> 8;
                    //max value for iReg is 0x0FFF because it's pointing to memory locations
                    //the maximum memory address is 4095 = 2 ^ 12 = 0x0FFF
                    iReg += registers[reg];
                    //if it overflows the possible addresses VF is set to 1
                    if(iReg > 0xFFF) registers[0xF] = 1;
                    else registers[0xF] = 0;
                    break;
                }

                case 0x000A:{
                    //std::cout << "0xFX0A done: " << std::endl;
                    uint16_t reg = getOperation(instruction, 0x0F00);
                    reg = reg >> 8;
                    //while a key is not pressed, the program should wait
                    //for the key to be pressed. This could be done by not incrementing PC
                    //until a key is pressed.
                    uint8_t key = getKey();
                    if(key != -1){
                        registers[reg] = key;
                        return true;
                    }else return false;
                    break;
                }
                case 0x0029:{
                    //std::cout << "0xFX29 done: " << std::endl;
                    uint16_t reg = getOperation(instruction, 0x0F00);
                    reg = reg >> 8;
                    uint8_t initialSpritesAddress = 0x50;
                    uint8_t spriteHeight = 0x05;
                    //iReg = memAddStart + (charHex * 5)
                    iReg = initialSpritesAddress + (registers[reg] * spriteHeight);
                    break;
                }
                case 0x0033:{ 
                    uint16_t reg = getOperation(instruction, 0x0F00);
                    reg = reg >> 8;
                    uint8_t regValue = registers[reg];
                    //assuming that Vx holds up to 3 digits 0 - 
                    ram[iReg] = regValue / 100;
                    ram[iReg + 1] = (regValue / 10) % 10;
                    ram[iReg + 2] = regValue % 10;
                    break;
                }
                case 0x0055:{
                    uint16_t reg = getOperation(instruction, 0x0F00);
                    reg = reg >> 8;
                    for(int i = 0; i <= reg; i++){
                        ram[iReg + i] = registers[i];
                    }
                    
                    break;
                }
                case 0x0065:{
                    uint16_t reg = getOperation(instruction, 0x0F00);
                    reg = reg >> 8;
                    for(int i = 0; i <= reg; i++){
                        registers[i] = ram[iReg + i];
                    }
                    
                    break;
                }
            }

            break;
        }
    }
    return true;
}

uint8_t Chip8::getKey(){
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_1]){
        return 0x0;
    }else if (state[SDL_SCANCODE_2]){
        return 0x1;
    }else if (state[SDL_SCANCODE_3]){
        return 0x2;
    }else if (state[SDL_SCANCODE_4]){
        return 0x3;
    }else if (state[SDL_SCANCODE_Q]){
        return 0x4;
    }else if (state[SDL_SCANCODE_W]){
        return 0x5;
    }else if (state[SDL_SCANCODE_E]){
        return 0x6;
    }else if (state[SDL_SCANCODE_R]){
        return 0x7;
    }else if (state[SDL_SCANCODE_A]){
        return 0x8;
    }else if (state[SDL_SCANCODE_S]){
        return 0x9;
    }else if (state[SDL_SCANCODE_D]){
        return 0xA;
    }else if (state[SDL_SCANCODE_F]){
        return 0xB;
    }else if (state[SDL_SCANCODE_Z]){
        return 0xC;
    }else if (state[SDL_SCANCODE_X]){
        return 0xD;
    }else if (state[SDL_SCANCODE_C]){
        return 0xE;
    }else if (state[SDL_SCANCODE_V]){
        return 0xF;
    }else return -1;
}
void Chip8::updateTimers(){
    if(delayTimer > 0) delayTimer--;
    if (soundTimer > 0) {
        soundTimer--;
        SDL_PauseAudioDevice(dev, 0); // Play sound
    } else {
        SDL_PauseAudioDevice(dev, 1); // Pause sound
    }
}
void Chip8::run(){
    uint16_t instruction;
    //int i = 7;//debug
    while(pc <= 4096){//debug
        uint16_t timeBeforeFrame = SDL_GetPerformanceCounter();
        for(uint64_t i = 0; i < INSTRUCTIONS_PER_SECOND / 60; i++){
            //fetch with big endian convention
            //each instruction is 2 bytes, the first byte is stored most-significant-byte first
            instruction = ram[pc] << 8;
            instruction += ram[pc+1];
            //decode and execute
            bool canIncrementPc = decodeAndExecute(instruction);
            //some instructions (like JUMPS) don't need to increment PC immediatly after jumping
            (canIncrementPc == true) ?  pc += 2 : pc += 0;
        }
        uint16_t timeAfterFrame = SDL_GetPerformanceCounter();
        const double elapsedTime = (double) ((timeAfterFrame - timeBeforeFrame) / 1000) / SDL_GetPerformanceFrequency();
        SDL_Delay(16.67f > elapsedTime ? 16.67f - elapsedTime : 0);
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT) break;
        updateTimers();
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(dev);
    SDL_Quit();
}

Chip8::~Chip8(){

}