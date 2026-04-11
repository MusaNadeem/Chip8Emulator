#include <fstream>
#include <string>
#include <cstdint>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include "Chip8.hpp"

bool Chip8::LoadRom(const std::string& filename){
    
    srand(time(NULL));
    std::ifstream file(filename, std::ios::binary);
    
    if (file.is_open()){

        file.seekg(0,std::ios::end);
        long size = file.tellg();
        if (size <= 3584)
        {
            {
                file.seekg(0,std::ios::beg);
                file.read(reinterpret_cast<char*>(&memory[0x200]),size);
            }
            
            file.close();
            return true;   
        }
        
    }
    return false;
};

void Chip8::Cycle(){
    
    opcode = (memory[PC] << 8) | memory[PC + 1];

    PC += 2;

    uint8_t nibble = (opcode & 0xF000) >> 12;


    switch(nibble){

        case 0x0:{

            uint8_t lastByte = (opcode & 0x00FF);

            switch(lastByte){
                //0nnn
                case 0xE0:
                    std::memset(display, 0, sizeof(display));
                    break;

                case 0xEE:
                    sp--;
                    PC = stack[sp];
                    break;
            }
        }

            break;
        case 0x1:
        
            //1nnn: jump to location nnn
            PC = opcode & 0x0FFF;
            break;


        case 0x2:
            //2nnn: call subroutine at nnn
            if(sp <= 15){

                stack[sp] = PC;
                sp++;
                PC = opcode & 0x0FFF;

            }
            break;


        case 0x3:
            //3xkk: Skip next instruction if Vx = kk
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
            {
                PC += 2;
            }
            
            break;
        case 0x4:
            //4xkk: Skip next instruction if Vx != kk
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
            {
                PC += 2;
            }
            break;

        case 0x5:
            //5xy0: Skip next instruction if Vx = Vy
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
            {
                PC += 2;
            }

            break;
        case 0x6:

            //6xkk: set Vx = kk
            
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            break;

        case 0x7:

            //7xkk: set Vx = Vx + kk
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            break;

        case 0x8:{

            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            
            switch(opcode & 0x000F){
                
                case 0x0:
                    V[x] = V[y];
                    break;

                case 0x1:
                    V[x] = V[x] | V[y];
                    break;

                case 0x2:
                    V[x] = V[x] & V[y];
                    break;

                case 0x3:
                    V[x] = V[x] ^ V[y];
                    break;

                case 0x4:
                    if ((V[x] + V[y]) > 0xFF)
                    {
                        V[x] = V[x] + V[y];
                        V[15] = 0x1;
                    }
                    else{
                        V[x] = V[x] + V[y];
                        V[15] = 0x0;
                    }
                    break;

                case 0x5:
                    if (V[x] >= V[y])
                    {
                        V[x] = V[x] - V[y];
                        V[15] = 0x1;
                    }
                    else{
                        V[x] = V[x] - V[y];
                        V[15] = 0x0;
                    }
                    break;

                case 0x6:{

                    uint8_t lsb = V[x] & 0x1;
                    V[x] = V[x] >> 1;
                    
                    V[15] = lsb;
                    break;
                }

                case 0x7:
                    if (V[y] >= V[x])
                    {
                        V[x] = V[y] - V[x];
                        V[15] = 1;
                    }
                    else{
                        V[x] = V[y] - V[x];
                        V[15] = 0;
                    }
                    
                    break;

                case 0xE:{
                    uint8_t msb = (V[x] & 0x80) >> 7;
                    V[x] = V[x] << 1;
                    V[15] = msb;
                    break;               
                }
                }
                
                break;
            }

        case 0x9:
            //9xy0: Skip next instruction if Vx != Vy
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
            {
                PC += 2;
            }
            break;

        case 0xA:
            //Annn: set I to nnn;
            I = opcode & 0x0FFF;
            break;

        case 0xB:
            //Bnnn: jump to nnn + V[0]
            PC = (opcode & 0x0FFF) + V[0];
            break;

        case 0xC:
        
            //Cxkk:Vx = rand & kk
            V[(opcode & 0x0F00) >> 8] = (std::rand() % 256) & (opcode & 0x00FF); 
            break;

        case 0xD:{
            uint8_t D_x = (opcode & 0x0F00) >> 8;
            uint8_t D_y = (opcode & 0x00F0) >> 4;
            uint8_t D_n = (opcode & 0x000F);

            uint8_t start_x = V[D_x];
            uint8_t start_y = V[D_y];
            V[15] = 0;

            for (int i = 0; i < D_n; i++)
            {
                uint8_t current_byte = memory[i+I];
                for (int j = 0; j < 8; j++)
                {
                    uint8_t current_bit = (current_byte & (0x80>> j)) >> (7-j);

                    if (current_bit == 1)
                    {
                        uint8_t x_final = (start_x + j) % 64;
                        uint8_t y_final = (start_y + i) % 32;
                        int index = (y_final * 64) + x_final;
                        
                        if(display[index] == 1){
                            V[15] = 0x1;
                        }
    
                        display[index] ^= current_bit;
                    }
                }
               
                
            }
            

            break;

        }
            

        case 0xE:{
            uint8_t E_last2 = opcode & 0x00FF;
            uint8_t E_x = (opcode & 0x0F00) >> 8;
            
            switch(E_last2){
                case 0x9E:
                    if(keypad[V[E_x]]){
                        PC = PC + 2;
                    }
                    break;
                case 0xA1:
                    if(!keypad[V[E_x]]){
                        PC = PC + 2;
                    }
                    break;

            }
            
            
            break;
        }

        case 0xF:{
            uint8_t F_last2 = opcode & 0x00FF;
            uint8_t F_x = (opcode & 0x0F00) >> 8;
            switch (F_last2)
            {
                case 0x07:
                    V[F_x] = delayTimer;

                    break;

                case 0x0A:{
                    bool keyPressed = false;
                    for (int i = 0; i < 16; i++)
                    {
                        if (keypad[i])
                        {
                            keyPressed = true;
                            V[F_x] = i;
                        }
                    }
                    if (!keyPressed)
                    {
                        PC = PC - 2;
                    }
                    break;
                }
                
                
                case 0x15:
                
                    delayTimer = V[F_x];
                    break;

                case 0x18:
                    soundTimer = V[F_x];

                    break;
                case 0x1E:
                    I = I + V[F_x];
                    break;
                case 0x29:
                    I = 0x050 + (V[F_x] * 5);
                    break;
                case 0x33:{
                    uint8_t temp = V[F_x];
                    memory[I] = temp / 100;
                    temp = temp % 100;
                    memory[I+1] = temp / 10;
                    temp = temp % 10;
                    memory[I+2] = temp;

                     break;
                }
                case 0x55:
                    for (int i = 0; i <= F_x; i++)
                    {
                        memory[I + i] = V[i]; 
                    }
                    
                    break;
                case 0x65:
                    for (int i = 0; i <= F_x; i++)
                    {
                        V[i] = memory[I + i]; 
                    }
                    break;
                
            }
            break;
        }

    }


}


void Chip8::TickTimers(){
    if (soundTimer > 0)
    {
        soundTimer--;
    }
    if (delayTimer > 0)
    {
        delayTimer--;
    }
}