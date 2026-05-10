#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

class LoginScreen {
private:
    sf::RenderWindow& window;
    sf::Font& font;
    string playerName;

public:
    LoginScreen(sf::RenderWindow& window, sf::Font& font);
    string run();
};

#endif