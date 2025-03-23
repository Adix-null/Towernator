#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(700, 700)), "Testing");

    while (window->isOpen())
    {
        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>()) 
            {
                window->close();
            }
        }

        //render 

        window->clear();

        //draw stuff

        window->display();
    }

    delete window;
    return 0;
}
