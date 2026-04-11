#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>
#include <string>


const unsigned int FONTSET_SIZE = 80;
const uint8_t FONTSET[80] = {
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

class Chip8{


    public:
    Chip8(){
        //standard PC Start Address for CHIP8
        this->PC = 0x200;

        //Clearing Memory
        for (int i = 0; i < 4096; i++)
        {
            this->memory[i] = 0;
            
        }

        //Clearing Registers
        for (int i = 0; i < 16; i++)
        {
            this->V[i] = 0;
        }

        for (int i = 0; i < 80; i++)
        {
            this->memory[0x050 + i] = FONTSET[i];
        }
        
        
        
    };

    // Loading the game
    bool LoadRom(const std::string& filename);
    
    // Fetch-Decode-Execute Cycle
    void Cycle();

    // Decrement Timers
    void TickTimers();


    uint32_t display[64*32]{}; // 64 x 32 pixel display
    uint8_t keypad[16]{}; // Keypad mapped to [0 to F]
    uint8_t soundTimer{}; // Sound timer ticking at 60Hz
    uint8_t delayTimer{}; // Delay timer ticking at 60Hz

    private:
    uint8_t V[16]{}; // 16 regular use registers with a flag register VF
    uint16_t stack[16]{}; // Stacks for storing memory addresses during subroutines
    uint8_t memory[4096]{}; // 4KB memory
    uint8_t sp{}; // Stack pointer (memory pointer for stack)
    uint16_t I{}; // Index Register
    uint16_t PC{}; // Program Counter
    uint16_t opcode{}; // Stores the opcode

};
#endif
