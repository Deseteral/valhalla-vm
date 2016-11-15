#include <random>
#include <SFML/Graphics.hpp>

#include "valhalla\Display.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 80
#define SCALE 6

void renderDisplayToTexture(Display* display, sf::Texture* texture)
{
    const static sf::Color palette[] = {
        sf::Color(0, 0, 0),
        sf::Color(157, 157, 157),
        sf::Color(255, 255, 255),
        sf::Color(190, 38, 51),
        sf::Color(224, 111, 139),
        sf::Color(73, 60, 43),
        sf::Color(164, 100, 34),
        sf::Color(235, 137, 49),
        sf::Color(247, 226, 107),
        sf::Color(47, 72, 78),
        sf::Color(68, 137, 26),
        sf::Color(163, 206, 39),
        sf::Color(27, 38, 50),
        sf::Color(0, 87, 132),
        sf::Color(49, 162, 242),
        sf::Color(178, 220, 239),
        sf::Color(255, 0, 255)
    };

    sf::Uint8 pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT * 4];

    for (int i = 0; i < (DISPLAY_WIDTH * DISPLAY_HEIGHT); i++)
    {
        pixels[i * 4]     = palette[display->buffer[i]].r;
        pixels[i * 4 + 1] = palette[display->buffer[i]].g;
        pixels[i * 4 + 2] = palette[display->buffer[i]].b;
        pixels[i * 4 + 3] = palette[display->buffer[i]].a;
    }

    texture->update(pixels);
}

int main()
{
    // TODO: Move this to a VM class that describes the state of app
    Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

    // Display to SFML layer
    sf::Texture displayTexture;
    displayTexture.create(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    renderDisplayToTexture(&display, &displayTexture);

    sf::Sprite displaySprite;
    displaySprite.setTexture(displayTexture);
    displaySprite.setScale(sf::Vector2f(SCALE, SCALE));

    // RANDOM
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 15);

    // Setting up render window
    sf::RenderWindow window(
        sf::VideoMode(DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE),
        "Valhalla VM"
    );

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++)
        {
            display.buffer[i] = (u8)(distribution(generator));
        }

        renderDisplayToTexture(&display, &displayTexture);
        window.draw(displaySprite);

        window.display();
    }

    return 0;
}
