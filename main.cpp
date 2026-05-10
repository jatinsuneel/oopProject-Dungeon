#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <string>

#include "LoginScreen.h"
#include "Game.h"

using namespace std;

int main() {
    srand((unsigned int)time(0));

    sf::RenderWindow window(
        sf::VideoMode({900, 700}),
        "Dungeon Escape",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        // fallback: try common Windows path
        if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            return -1;  // no font found
        }
    }

    LoginScreen login(window, font);
    string playerName = login.run();

    if (!window.isOpen() || playerName.empty()) return 0;

    Game game(window, font, playerName);
    game.start();

    return 0;
}