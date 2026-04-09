#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>
#include <string>

class Chip8{

    public:
    uint32_t display[64*32]{};
    uint8_t keypad[16]{};
    uint8_t soundTimer{};
    uint8_t delayTimer{};

    private:
    uint8_t V[16]{};
    uint16_t stack[16]{};
    uint8_t memory[4096];
    uint8_t sp{};
    uint16_t I{};
    uint16_t PC{};
    uint16_t opcode{}; 

};
#endif
