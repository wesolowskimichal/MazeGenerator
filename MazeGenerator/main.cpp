#include "Screen.h"
int main()
{
    Screen* screen = new Screen("Name", 250.f, 250.f);
    sf::RenderWindow* window = &screen->_window();
    window->setFramerateLimit(60);
    while (window->isOpen())
    {

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();

        }
        //screen->create_Grid(10, 10);
        screen->update();
    }

    return 0;
}