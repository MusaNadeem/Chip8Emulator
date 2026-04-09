#include <fstream>
#include <string>
#include <cstdint>
#include <ctime>
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

        case 0x0:


        case 0x1:
        
            //1nnn: jump to location nnn
            PC = opcode & 0x0FFF;
            break;


        case 0x2:
            //2nnn: call subroutine at nnn
            if(sp < 15){

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

        case 0x8:

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

        case 0xD:

        case 0xE:

        case 0xF:

    }


}


