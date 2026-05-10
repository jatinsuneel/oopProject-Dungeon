#ifndef DUNGEON_H
#define DUNGEON_H

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Monster.h"

using namespace std;

class Dungeon {
private:
    vector<vector<vector<char>>> levels;
    int currentLevel;

public:
    Dungeon();

    int getCurrentLevelNumber() const;
    int getTotalLevels() const;
    bool isLastLevel() const;
    void nextLevel();

    bool isWall(int x, int y) const;
    char getCell(int x, int y) const;
    void setCell(int x, int y, char value);

    int getWidth() const;
    int getHeight() const;

    // SFML display - replaces old console display
    void display(const Player& player,
                 const vector<Monster>& monsters,
                 sf::RenderWindow& window,
                 sf::Font& font,
                 const string& playerName) const;
};

#endif