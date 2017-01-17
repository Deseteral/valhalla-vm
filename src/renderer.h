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
