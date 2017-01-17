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

    VM* vm = new VM(vmConfig);
    VMInputState inputState;

    // Setting up render window
    sf::RenderWindow window(
        sf::VideoMode(DISPLAY_WIDTH * SCALE + 360, DISPLAY_HEIGHT * SCALE),
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
        window.clear();

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        // stdin
        char input = ' ';
        VMInputState newState;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
            newState.keyState[0] = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            newState.keyState[1] = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            newState.keyState[2] = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            newState.keyState[3] = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            newState.keyState[4] = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
            newState.keyState[5] = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
            newState.keyState[6] = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
            newState.keyState[7] = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
            newState.keyState[8] = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
            newState.keyState[9] = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            newState.keyState[10] = true;

        for (int i = 0; i < 11; i++)
        {
            if (!inputState.keyState[i] && newState.keyState[i])
                input = i == 10 ? '\n' : ((int)'0' + i);
        }

        inputState = newState;

        // GUI Rendering
        ImGui::SFML::Update(deltaClock.restart());

        ImGui::Begin("Valhalla VM");
        if (ImGui::CollapsingHeader("Simulation control"))
        {
            ImGui::InputText("vasm file path", assemblerPathBuffer, 256);

            if (ImGui::Button("Assemble"))
            {
                Assembler assembler(assemblerPathBuffer);
                sourceCode = assembler.getSourceCode();

                assembler.compile();

                std::vector<u8>* bytecode = assembler.getBytecode();
                vm->loadIntoMemory(bytecode);
            }

            ImGui::SameLine();
            if (ImGui::Button("Halt"))
                vm->halt = !vm->halt;

            ImGui::SameLine();
            if (ImGui::Button("Reset VM"))
            {
                delete vm;
                vm = new VM(vmConfig);
            }
        }

        if (ImGui::CollapsingHeader("VM internals"))
        {
            ImGui::Text("X: %d", vm->registers[0]); ImGui::SameLine();
            ImGui::Text("Y: %d", vm->registers[1]); ImGui::SameLine();
            ImGui::Text("C: %d", vm->registers[2]); ImGui::SameLine();
            ImGui::Text("W: %d", vm->registers[3]); ImGui::SameLine();
            ImGui::Text("A: %d", vm->registers[4]); ImGui::SameLine();
            ImGui::Text("B: %d", vm->registers[5]);
            ImGui::Text("J: %d", vm->registers[6]); ImGui::SameLine();
            ImGui::Text("K: %d", vm->registers[7]); ImGui::SameLine();
            ImGui::Text("L: %d", vm->registers[8]);
            ImGui::Spacing();

            // Memory dump
            for (uint my = 0; my < 16; my++)
            {
                string mbuf = "";
                for (uint mx = 0; mx < 16; mx++)
                {
                    uint memoryValue = (uint)vm->memory[my * 16 + mx];

                    std::stringstream stream;
                    stream << std::hex << memoryValue;

                    string hexValue(stream.str());

                    mbuf += hexValue;
                    for (int fill = (3 - hexValue.size()); fill != 0; fill--)
                        mbuf += " ";
                }
                ImGui::Text(mbuf.c_str());
            }
        }

        if (ImGui::CollapsingHeader("Source code"))
        {
            ImGui::Text(sourceCode.c_str());
        }

        ImGui::End();

        if (!vm->halt)
            vm->tick(input);

        renderDisplayToTexture(vm->display, &displayTexture, &fontSprite);
        window.draw(displaySprite);

        ImGui::Render();
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
