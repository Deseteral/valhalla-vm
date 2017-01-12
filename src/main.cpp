#include "imgui/imgui.h"
#include "imgui/imgui-sfml.h"

#include <iostream>
#include <SFML/System/Clock.hpp>

#include "renderer.h"
#include "valhalla/Assembler.h"

int main()
{
    VMConfig vmConfig = {
        255,                          // memory size
        DISPLAY_WIDTH, DISPLAY_HEIGHT // display resolution
    };

    VM vm(vmConfig);

    // Setting up render window
    sf::RenderWindow window(
        sf::VideoMode(DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE),
        "Valhalla VM"
    );

    ImGui::SFML::Init(window);

    // Rendering Display to SFML layer
    sf::RenderTexture displayTexture;
    displayTexture.create(DISPLAY_WIDTH, DISPLAY_HEIGHT);

    sf::Sprite displaySprite;
    displaySprite.setTexture(displayTexture.getTexture());
    displaySprite.setScale(SCALE, SCALE);

    // Loading font texture
    sf::Texture fontTexture;
    if (!fontTexture.loadFromFile("res/font.png"))
    {
        std::cerr << "Couldn't load font file" << std::endl;
        return 1;
    }

    sf::Sprite fontSprite(fontTexture);

    // ImGui resource allocation
    char* assemblerPathBuffer = new char[256]();

    // Main loop
    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(deltaClock.restart());

        ImGui::InputText("vasm file path", assemblerPathBuffer, 256);

        if (ImGui::Button("Assemble")) {
            Assembler assembler(assemblerPathBuffer);
            assembler.compile();

            std::vector<u8>* bytecode = assembler.getBytecode();
            vm.loadIntoMemory(bytecode);
        }

        if (!vm.halt)
            vm.tick();

        renderDisplayToTexture(vm.display, &displayTexture, &fontSprite);
        window.draw(displaySprite);

        ImGui::Render();
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
