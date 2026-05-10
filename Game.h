#ifndef GAME_H
#define GAME_H

#include <vector>
#include <stack>
#include <utility>
#include <string>
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Monster.h"
#include "Dungeon.h"
#include "InventoryADT.h"

using namespace std;

class Game {
private:
    Player player;
    vector<Monster> monsters;
    Dungeon dungeon;
    stack<pair<int, int>> previousMoves;
    vector<int> scores;
    InventoryADT extraInventory;
    bool gameOver;

    sf::RenderWindow& window;
    sf::Font& font;
    string playerName;

    void showGUIMessage(const string& title, const string& body);
    void pauseScreen();

public:
    Game(sf::RenderWindow& window, sf::Font& font, const string& playerName);
    ~Game();

    void showInstructions();
    void start();
    void handleInput(char input);
    void movePlayer(int dx, int dy);
    void undoMove();

    int distanceToPlayer(int x, int y);
    bool isMonsterPositionUsed(int x, int y, int currentMonsterIndex);

    void moveMonstersTowardPlayer();
    bool handleMonsterAttack();

    void setupMonstersForLevel();
    void resetForNextLevel();
    void resetCurrentLevel();
    void checkGameStatus();

    int recursiveBonus(int n);
    void endGame();

    friend void showProjectInfo(Game& game);
};

void showProjectInfo(Game& game);

#endif
