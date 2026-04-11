#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "Chip8.hpp"
#include <string>


// implements the sfml window and manages timers and cpu frequency
int main(int argc, char* argv[]){
    // ensures rom argument is given 
    if (argc <2)
    {
        return -1;
    }

    // stores ROM Path
    std::string romPath = argv[1];
    Chip8 chip8;
    chip8.LoadRom(romPath);
    
    // sound creation
    sf::Music beep;
        if (!beep.openFromFile("include/beep.wav"))
        return EXIT_FAILURE;
    
    // window creation
    sf::RenderWindow window(sf::VideoMode({960,480}), "Chip8 Emulator");
    window.setFramerateLimit(60);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            
        }
        for (int i = 0; i < 16; i++)
        {
            chip8.keypad[i] =0;
        }
        // input detection
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
        {
            chip8.keypad[1] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
        {
            chip8.keypad[2] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
        {
            chip8.keypad[3] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
        {
            chip8.keypad[0xC] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        {
            chip8.keypad[4] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            chip8.keypad[5] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
        {
            chip8.keypad[6] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
        {
            chip8.keypad[0xD] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            chip8.keypad[7] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            chip8.keypad[8] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            chip8.keypad[9] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F))
        {
            chip8.keypad[0xE] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
        {
            chip8.keypad[0xA] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
        {
            chip8.keypad[0] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
        {
            chip8.keypad[0xB] = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V))
        {
            chip8.keypad[0xF] = 1;
        }

        window.clear(sf::Color::Black);
        if (chip8.soundTimer > 0)
        {
            beep.play();
        }
        

        for (int i = 0; i < 12; i++)
        {
            chip8.Cycle();
        }
        
        for (int i = 0; i < (64*32); i++)
        
        {
            if (chip8.display[i] == 1)
            {
                sf::RectangleShape pixel(sf::Vector2f(15,15 ));
                pixel.setPosition(15*(i % 64),15*(i/64));
                window.draw(pixel);   
            }  
        }
        window.display();
        chip8.TickTimers();
    }

    return 0;
}