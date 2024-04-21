#include <iostream>
#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

using namespace sf;
using namespace std;


int main() {
    int pixelWidth = VideoMode::getDesktopMode().width;
    int pixelHeight = VideoMode::getDesktopMode().height;
    VideoMode vm(pixelWidth, pixelHeight);
    RenderWindow window(vm, "Mandelbrot Set", Style::Default);

    View view;
    view.setSize(pixelWidth, pixelHeight);
    view.setCenter(VideoMode::getDesktopMode().width / 2, VideoMode::getDesktopMode().height / 2);
    window.setView(view);

    ComplexPlane complexPlane(pixelWidth, pixelHeight);


    Text text;

    Font font;
    font.loadFromFile("fonts/arial.ttf");
    text.setFont(font);

    bool update = true;

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            // **** LEFT CLICK ZOOM OUT ****
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    cout << "the left button was pressed\n";
                    cout << "mouse x: " << event.mouseButton.x << endl;
                    cout << "mouse y: " << event.mouseButton.y << endl;

                    // Call zoomOut
                    // Call setCenter
                    complexPlane.setCenter({(float)event.mouseButton.x, (float)event.mouseButton.y});

                    update = true;

                }
            }
            // **** RIGHT CLICK ZOOM IN ****
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Right) {
                    cout << "the right button was pressed\n";
                    cout << "mouse x: " << event.mouseButton.x << endl;
                    cout << "mouse y: " << event.mouseButton.y << endl;

                    // Call zoomIn
                    // Call setCenter
                    complexPlane.setCenter({(float)event.mouseButton.x, (float)event.mouseButton.y});

                    update = true;

                }
            }

            if (event.type == Event::MouseMoved) {
                cout << "the mouse moved\n";
                    cout << "mouse x: " << event.mouseMove.x << endl;
                    cout << "mouse y: " << event.mouseMove.y << endl;

                    // Call setMouseLocation 
                    complexPlane.setMouseLocation({(float)event.mouseMove.x, (float)event.mouseMove.y});
                    update = true;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();

        // Update

        if (update) {
            complexPlane.updateRender();
            complexPlane.loadText(text);
            update = false;
        }

        // Draw

        window.clear();
        window.draw(complexPlane);
        window.draw(text);
        window.display();
    }
}