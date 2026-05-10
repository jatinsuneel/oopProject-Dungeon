#include "LoginScreen.h"
#include <cmath>

LoginScreen::LoginScreen(sf::RenderWindow& window, sf::Font& font)
    : window(window), font(font), playerName("") {}

string LoginScreen::run() {
    sf::Vector2u sz = window.getSize();
    float W = (float)sz.x;
    float H = (float)sz.y;

    sf::Clock clock;
    bool done = false;

    while (window.isOpen() && !done) {

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return "";
            }
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Enter && !playerName.empty()) {
                    done = true;
                } else if (key->code == sf::Keyboard::Key::Backspace && !playerName.empty()) {
                    playerName.pop_back();
                }
            }
            if (const auto* txt = event->getIf<sf::Event::TextEntered>()) {
                char c = (char)txt->unicode;
                if (c >= 32 && c < 127 && (int)playerName.size() < 14) {
                    playerName += c;
                }
            }
        }

        float t = clock.getElapsedTime().asSeconds();
        float pulse = 0.5f + 0.5f * std::sin(t * 2.5f);

        window.clear(sf::Color(10, 10, 22));

        // Background decorative grid
        for (int i = 0; i < 20; i++) {
            sf::RectangleShape line({W, 1.f});
            line.setPosition({0.f, i * (H / 20.f)});
            line.setFillColor(sf::Color(30, 30, 55, 60));
            window.draw(line);
        }
        for (int i = 0; i < 30; i++) {
            sf::RectangleShape line({1.f, H});
            line.setPosition({i * (W / 30.f), 0.f});
            line.setFillColor(sf::Color(30, 30, 55, 40));
            window.draw(line);
        }

        // Panel
        float panelW = 500.f;
        float panelH = 360.f;
        float panelX = (W - panelW) / 2.f;
        float panelY = (H - panelH) / 2.f;

        sf::RectangleShape panel({panelW, panelH});
        panel.setPosition({panelX, panelY});
        panel.setFillColor(sf::Color(16, 16, 32, 230));
        panel.setOutlineColor(sf::Color(
            (uint8_t)(0 + (uint8_t)(40.f * pulse)),
            (uint8_t)(160 + (uint8_t)(60.f * pulse)),
            255,
            200
        ));
        panel.setOutlineThickness(2.f);
        window.draw(panel);

        // Title
        sf::Text title(font, "DUNGEON ESCAPE", 42);
        title.setFillColor(sf::Color(0, 210, 255));
        title.setStyle(sf::Text::Bold);
        sf::FloatRect tb = title.getLocalBounds();
        title.setOrigin({tb.size.x / 2.f, tb.size.y / 2.f});
        title.setPosition({W / 2.f, panelY + 60.f});
        window.draw(title);

        // Subtitle
        sf::Text sub(font, "Enter your name to begin", 18);
        sub.setFillColor(sf::Color(140, 140, 200));
        sf::FloatRect sb2 = sub.getLocalBounds();
        sub.setOrigin({sb2.size.x / 2.f, sb2.size.y / 2.f});
        sub.setPosition({W / 2.f, panelY + 115.f});
        window.draw(sub);

        // Divider
        sf::RectangleShape divLine({panelW - 60.f, 1.f});
        divLine.setPosition({panelX + 30.f, panelY + 145.f});
        divLine.setFillColor(sf::Color(0, 180, 255, 80));
        window.draw(divLine);

        // Input label
        sf::Text label(font, "Player Name", 16);
        label.setFillColor(sf::Color(100, 140, 200));
        label.setPosition({panelX + 70.f, panelY + 168.f});
        window.draw(label);

        // Input box
        sf::RectangleShape inputBox({panelW - 140.f, 46.f});
        inputBox.setPosition({panelX + 70.f, panelY + 192.f});
        inputBox.setFillColor(sf::Color(22, 22, 45));
        inputBox.setOutlineColor(sf::Color(
            (uint8_t)(40.f * pulse),
            (uint8_t)(180.f + 60.f * pulse),
            255
        ));
        inputBox.setOutlineThickness(2.f);
        window.draw(inputBox);

        // Input text with cursor
        string displayStr = playerName;
        if ((int)(t * 2) % 2 == 0) displayStr += "|";
        sf::Text inputTxt(font, displayStr, 22);
        inputTxt.setFillColor(sf::Color::White);
        inputTxt.setPosition({panelX + 84.f, panelY + 202.f});
        window.draw(inputTxt);

        // Start or type hint
        if (!playerName.empty()) {
            sf::Text hint(font, "Press ENTER to Start", 17);
            hint.setFillColor(sf::Color(
                60,
                (uint8_t)(200.f + 55.f * pulse),
                100
            ));
            sf::FloatRect hb = hint.getLocalBounds();
            hint.setOrigin({hb.size.x / 2.f, hb.size.y / 2.f});
            hint.setPosition({W / 2.f, panelY + 295.f});
            window.draw(hint);
        } else {
            sf::Text hint(font, "Type your name above", 16);
            hint.setFillColor(sf::Color(80, 80, 110));
            sf::FloatRect hb = hint.getLocalBounds();
            hint.setOrigin({hb.size.x / 2.f, hb.size.y / 2.f});
            hint.setPosition({W / 2.f, panelY + 295.f});
            window.draw(hint);
        }

        // Legend
        sf::Text legend(font, "P = You    M = Monster    T = Treasure    E = Exit", 14);
        legend.setFillColor(sf::Color(80, 80, 120));
        sf::FloatRect leg = legend.getLocalBounds();
        legend.setOrigin({leg.size.x / 2.f, leg.size.y / 2.f});
        legend.setPosition({W / 2.f, H - 30.f});
        window.draw(legend);

        window.display();
    }

    return playerName;
}