#include "ComplexPlane.h"

using namespace sf;
using namespace std;


int main()
{
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf"))
    {
        cerr << "Error loading font" << endl;
        return -1;
    }

    Text sideText;
    sideText.setFont(font);

    RenderWindow window(VideoMode(W, H), "Mandelbrot Set");

    ComplexPlane complexPlane(W, H); 

    Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Right)
                {
                    complexPlane.zoomOut();
                    complexPlane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
                if (event.mouseButton.button == Mouse::Left)
                {
                    complexPlane.zoomIn();
                    complexPlane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
            }

            if (event.type == Event::MouseMoved)
            {
                complexPlane.setMouseLocation(Vector2i(event.mouseMove.x, event.mouseMove.y));
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape))
            {
                window.close();
            }
        }

        complexPlane.updateRender();
        complexPlane.loadText(sideText);

        window.clear();
        window.draw(complexPlane);
        window.draw(sideText);
        window.display();
    }

    return 0;
}