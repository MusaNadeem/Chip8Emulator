#include <fstream>
#include <string>
#include <cstdint>
#include "Chip8.hpp"

bool Chip8::LoadRom(const std::string& filename){
    
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
            
            return true;   
        }
        
    }


    return false;
};
