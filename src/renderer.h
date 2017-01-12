#pragma once

#include <SFML/Graphics.hpp>

#include "valhalla/VM.h"

// TODO: Make this configurable
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240
#define DISPLAY_PIXEL_COUNT (DISPLAY_WIDTH * DISPLAY_HEIGHT)

#define DISPLAY_CHARS_HORIZONTAL (int)(DISPLAY_WIDTH / 8)
#define DISPLAY_CHARS_VERTICAL   (int)(DISPLAY_HEIGHT / 8)
#define DISPLAY_CHARS_COUNT      (DISPLAY_CHARS_VERTICAL * DISPLAY_CHARS_HORIZONTAL)

#define SCALE 3

static void renderDisplayToTexture(Display* display, sf::RenderTexture* render, sf::Sprite* fontSprite)
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
            int cx = ((int)(c % 16) * 8);
            int cy = ((int)(c / 16) * 8);

            int x = ((int)(i % DISPLAY_CHARS_HORIZONTAL) * 8);
            int y = ((int)(i / DISPLAY_CHARS_HORIZONTAL) * 8);

            fontSprite->setTextureRect(sf::IntRect(cx, cy, 8, 8));
            fontSprite->setPosition(x, y);

            render->draw(*fontSprite);
        }

        render->display();
    }
}
