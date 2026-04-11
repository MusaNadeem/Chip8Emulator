#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "Chip8.hpp"
#include <string>



int main(int argc, char* argv[]){

    if (argc <2)
    {
        return -1;
    }

    std::string romPath = argv[1];
    

    sf::RenderWindow window(sf::VideoMode({960,480}), "Chip8 Emulator");

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
        window.clear(sf::Color::Black);

        window.display();
        
    }
    
    


    return 0;
}