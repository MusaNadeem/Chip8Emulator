#include <fstream>
#include <string>
#include <cstdint>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include "Chip8.hpp"


    Chip8::Chip8(){
        //standard PC Start Address for CHIP8
        this->PC = 0x200;
        this->sp = 0;
        this->I=0;
        this->opcode = 0;
        this->delayTimer=0;
        this->soundTimer=0;

        //Clearing Memory
        for (int i = 0; i < 4096; i++)
        {
           this-> memory[i] = 0;
            
        }

        // Registers cleanup
        for (int i = 0; i < 16; i++)
        {
           this-> V[i] = 0;
        }

        // Memory cleanup
        for (int i = 0; i < 80; i++)
        {
           this-> memory[0x050 + i] = FONTSET[i];
        }

        // Stack cleanup
        for (int i = 0; i < 16; i++)
        {
           this-> stack[i] = 0;
        }

        // Display array cleanup
        for (int i = 0; i <(64*32); i++)
        {
            display[i] = 0;
        }

        // Keypad array cleanup
        for (int i = 0; i < 16; i++)
        {
            keypad[i] = 0;
        }
        
        
        
    };

// loads rom into memory
bool Chip8::LoadRom(const std::string& filename){
    
    srand(time(NULL)); // seed for random byte generation
    
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()){

        file.seekg(0,std::ios::end); // goes to end of file
        long size = file.tellg(); // counts no.of bytes from start
        if (size <= 3584)
        {
            {
                file.seekg(0,std::ios::beg); // goes to start of file
                file.read(reinterpret_cast<char*>(&memory[0x200]),size); 
            }
            
            file.close();
            return true;   
        }
        
    }
    return false;
};

void Chip8::Cycle(){

    // opcode fetch from memory
    opcode = (memory[PC] << 8) | memory[PC + 1]; // joins two 8-bit instructions

    PC += 2;

    // 
    uint8_t nibble = (opcode & 0xF000) >> 12;

    // switch case for opcodes decoding
    switch(nibble){

        case 0x0:{

            uint8_t lastByte = (opcode & 0x00FF);

            switch(lastByte){
                
                //0nnn: Jump to a machine code routine at nnn
                case 0xE0:
                    std::memset(display, 0, sizeof(display));
                    break;

                //00EE: Clear the display
                case 0xEE:
                    if (sp>0)
                    {
                        sp--;
                        PC = stack[sp];
                    }
                    
                    break;
            }
        }

            break;
        //1nnn: jump to location nnn
        
        case 0x1:
            PC = opcode & 0x0FFF;
            break;


        //2nnn: call subroutine at nnn
        case 0x2:
            if(sp <= 15){

                stack[sp] = PC;
                sp++;
                PC = opcode & 0x0FFF;

            }
            break;


        //3xkk: Skip next instruction if Vx = kk
        case 0x3:
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
            {
                PC += 2;
            }
            
            break;


        //4xkk: Skip next instruction if Vx != kk
        case 0x4:
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
            {
                PC += 2;
            }
            break;

        //5xy0: Skip next instruction if Vx = Vy
        case 0x5:
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
            {
                PC += 2;
            }

            break;

        //6xkk: set Vx = kk
        case 0x6:

            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            break;

        //7xkk: set Vx = Vx + kk
        case 0x7:

            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            break;


        case 0x8:{
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            
            switch(opcode & 0x000F){
                //8xy0: Set Vx = Vy
                case 0x0:
                    V[x] = V[y];
                    break;

                //8xy1: Set Vx = Vx OR Vy
                case 0x1:
                    V[x] = V[x] | V[y];
                    break;

                //8xy2: Set Vx = Vx AND Vy
                case 0x2:
                    V[x] = V[x] & V[y];
                    break;
                
                //8xy3: Set Vx = Vx XOR Vy
                case 0x3:
                    V[x] = V[x] ^ V[y];
                    break;

                //8xy4: Set Vx = Vx + Vy, set VF = carry
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

                //8xy5: Set Vx = Vx - Vy, set VF = NOT borrow
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

                //8xy6: Set Vx = Vx SHR 1
                case 0x6:{

                    uint8_t lsb = V[x] & 0x1;
                    V[x] = V[x] >> 1;
                    
                    V[15] = lsb;
                    break;
                }

                //8xy7: Set Vx = Vy - Vx, set VF = NOT borrow
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

                //8xyE: Set Vx = Vx SHL 1
                case 0xE:{
                    uint8_t msb = (V[x] & 0x80) >> 7;
                    V[x] = V[x] << 1;
                    V[15] = msb;
                    break;               
                }
                }
                
                break;
            }

        //9xy0: Skip next instruction if Vx != Vy
        case 0x9:
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
            {
                PC += 2;
            }
            break;

        //Annn: set I to nnn;
        case 0xA:
            I = opcode & 0x0FFF;
            break;

        //Bnnn: jump to nnn + V[0]
        case 0xB:
            PC = (opcode & 0x0FFF) + V[0];
            break;

        //Cxkk: Vx = rand & kk
        case 0xC:
        
            V[(opcode & 0x0F00) >> 8] = (std::rand() % 256) & (opcode & 0x00FF); 
            break;

        //Dxyn: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
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

                //Ex9E: Skip next instruction if key with the value of Vx is pressed
                case 0x9E:
                    if((V[E_x] <= 0xF) && (keypad[V[E_x]])){
                        PC = PC + 2;
                    }
                    break;
                
                //ExA1: Skip next instruction if key with the value of Vx is not pressed
                case 0xA1:
                    if((V[E_x] <= 0xF) && !keypad[V[E_x]]){
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
                //Fx07: Set Vx = delay timer value
                case 0x07:
                    V[F_x] = delayTimer;

                    break;
                
                //Fx0A: Wait for a key press, store the value of the key in Vx
                case 0x0A:{
                    bool keyPressed = false;
                    for (int i = 0; i < 16; i++)
                    {
                        if (keypad[i])
                        {
                            keyPressed = true;
                            V[F_x] = i;
                            break;
                        }
                    }
                    if (!keyPressed)
                    {
                        PC = PC - 2;
                    }
                    break;
                }
                
                //Fx15: Set delay timer = Vx
                case 0x15:
                
                    delayTimer = V[F_x];
                    break;

                //Fx18: Set sound timer = Vx
                case 0x18:
                    soundTimer = V[F_x];
                    break;

                //Fx1E: Set I = I + Vx
                case 0x1E:
                    I = I + V[F_x];
                    break;
                
                //Fx29: Set I = location of sprite for digit Vx
                case 0x29:
                    I = 0x050 + (V[F_x] * 5);
                    break;

                //Fx33: Store BCD representation of Vx in memory locations I, I+1, and I+2
                case 0x33:{
                    uint8_t temp = V[F_x];
                    memory[I] = temp / 100;
                    temp = temp % 100;
                    memory[I+1] = temp / 10;
                    temp = temp % 10;
                    memory[I+2] = temp;
                    break;
                }

                //Fx55: Store registers V0 through Vx in memory starting at location I
                case 0x55:
                    for (int i = 0; i <= F_x; i++)
                    {
                        memory[I + i] = V[i]; 
                    }
                    
                    break;
                
                //Fx65: Read registers V0 through Vx from memory starting at location I
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

// Decrements timers
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