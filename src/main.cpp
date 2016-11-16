#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>

#include "valhalla/Display.h"

// TODO: Make this configurable
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240
#define DISPLAY_PIXEL_COUNT (DISPLAY_WIDTH * DISPLAY_HEIGHT)

#define DISPLAY_CHARS_HORIZONTAL (int)(DISPLAY_WIDTH / 8)
#define DISPLAY_CHARS_VERTICAL   (int)(DISPLAY_HEIGHT / 8)
#define DISPLAY_CHARS_COUNT      (DISPLAY_CHARS_VERTICAL * DISPLAY_CHARS_HORIZONTAL)

#define SCALE 3

void renderDisplayToTexture(Display* display, sf::RenderTexture* render, sf::Sprite* fontSprite)
{
    // TODO: Loading palette from resource file
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

    if (display->mode == GRAPHICS)
    {
        sf::Uint8 pixels[DISPLAY_PIXEL_COUNT * 4];

        for (int i = 0; i < DISPLAY_PIXEL_COUNT; i++)
        {
            pixels[i * 4] = palette[display->buffer[i]].r;
            pixels[i * 4 + 1] = palette[display->buffer[i]].g;
            pixels[i * 4 + 2] = palette[display->buffer[i]].b;
            pixels[i * 4 + 3] = palette[display->buffer[i]].a;
        }

        sf::Texture tempTexture;
        tempTexture.create(DISPLAY_WIDTH, DISPLAY_HEIGHT);
        tempTexture.update(pixels);
        sf::Sprite tempSprite(tempTexture);

        render->draw(tempSprite);
        render->display();
    }
    else if (display->mode == TEXT)
    {
        render->clear();

        for (int i = 0; i < DISPLAY_CHARS_COUNT; i++)
        {
            char c = display->buffer[i];
            int cx = (int)(c % 16);
            int cy = (int)(c / 16);

            int x = (int)(i % DISPLAY_CHARS_HORIZONTAL);
            int y = (int)(i / DISPLAY_CHARS_HORIZONTAL);

            fontSprite->setTextureRect(sf::IntRect(cx * 8, cy * 8, 8, 8));
            fontSprite->setPosition(x * 8, y * 8);

            render->draw(*fontSprite);
        }

        render->display();
    }
}

int main()
{
    // TODO: Move this to a VM class that describes the state of app
    Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

    // Display to SFML layer
    sf::RenderTexture displayTexture;
    displayTexture.create(DISPLAY_WIDTH, DISPLAY_HEIGHT);

    sf::Sprite displaySprite;
    displaySprite.setTexture(displayTexture.getTexture());
    displaySprite.setScale(SCALE, SCALE);

    // RANDOM
    std::default_random_engine generator;
    std::uniform_int_distribution<int> randomColor(0, 15);
    std::uniform_int_distribution<int> randomChar(33, 126);

    // Setting up render window
    sf::RenderWindow window(
        sf::VideoMode(DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE),
        "Valhalla VM"
    );

    // Loading font texture
    sf::Texture fontTexture;
    if (!fontTexture.loadFromFile("res/font.png"))
    {
        std::cerr << "Couldn't load font file" << std::endl;
        return 1;
    }

    sf::Sprite fontSprite(fontTexture);

    // Main loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (int i = 0; i < DISPLAY_PIXEL_COUNT; i++)
        {
            display.buffer[i] = (u8)(randomChar(generator));
        }

        //display.buffer[0] = 'h';
        //display.buffer[1] = 'e';
        //display.buffer[2] = 'l';
        //display.buffer[3] = 'l';
        //display.buffer[4] = 'o';

        renderDisplayToTexture(&display, &displayTexture, &fontSprite);
        window.draw(displaySprite);

        window.display();
    }

    return 0;
}
