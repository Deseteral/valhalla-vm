#include "imgui/imgui.h"
#include "imgui/imgui-sfml.h"

#include <iostream>
#include <sstream>
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
    string sourceCode = "";

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

        ImGui::Begin("Simulation control");
        ImGui::InputText("vasm file path", assemblerPathBuffer, 256);

        if (ImGui::Button("Assemble"))
        {
            Assembler assembler(assemblerPathBuffer);
            sourceCode = assembler.getSourceCode();

            assembler.compile();

            std::vector<u8>* bytecode = assembler.getBytecode();
            vm.loadIntoMemory(bytecode);
        }

        if (ImGui::Button("Halt"))
            vm.halt = !vm.halt;
        ImGui::End();

        ImGui::Begin("VM internals");
        ImGui::Text("X: %d", vm.registers[0]);
        ImGui::Text("Y: %d", vm.registers[1]);
        ImGui::Text("C: %d", vm.registers[2]);
        ImGui::Text("W: %d", vm.registers[3]);
        ImGui::Text("A: %d", vm.registers[4]);
        ImGui::Text("B: %d", vm.registers[5]);
        ImGui::Text("J: %d", vm.registers[6]);
        ImGui::Text("K: %d", vm.registers[7]);
        ImGui::Text("L: %d", vm.registers[8]);
        ImGui::Text("");

        // Memory dump
        for (uint my = 0; my < 16; my++)
        {
            string mbuf = "";
            for (uint mx = 0; mx < 16; mx++)
            {
                uint memoryValue = (uint)vm.memory[my * 16 + mx];

                std::stringstream stream;
                stream << std::hex << memoryValue;

                string hexValue(stream.str());

                mbuf += hexValue;
                for (int fill = (3 - hexValue.size()); fill != 0; fill--)
                    mbuf += " ";
            }
            ImGui::Text(mbuf.c_str());
        }
        ImGui::End();

        ImGui::Begin("Source code");
        ImGui::Text(sourceCode.c_str());
        ImGui::End();

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
