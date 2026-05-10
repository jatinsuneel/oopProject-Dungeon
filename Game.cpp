#include "Game.h"
#include "GameException.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <limits>
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace std;

void Game::showGUIMessage(const string& title, const string& body) {
    sf::Vector2u sz = window.getSize();
    float W = (float)sz.x;
    float H = (float)sz.y;

    float boxW = 460.f;
    float boxH = 200.f;
    float boxX = (W - boxW) / 2.f;
    float boxY = (H - boxH) / 2.f;

    sf::Clock clock;
    bool waiting = true;

    while (window.isOpen() && waiting) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) { window.close(); return; }
            if (const auto* k = event->getIf<sf::Event::KeyPressed>()) {
                if (k->code == sf::Keyboard::Key::Enter ||
                    k->code == sf::Keyboard::Key::Space) {
                    waiting = false;
                }
            }
        }

        float t = clock.getElapsedTime().asSeconds();
        float pulse = 0.5f + 0.5f * std::sin(t * 3.f);

        dungeon.display(player, monsters, window, font, playerName);

        sf::RectangleShape dim({W, H});
        dim.setFillColor(sf::Color(0, 0, 0, 160));
        window.draw(dim);

        sf::RectangleShape box({boxW, boxH});
        box.setPosition({boxX, boxY});
        box.setFillColor(sf::Color(14, 14, 30, 245));
        box.setOutlineColor(sf::Color(
            (uint8_t)(40.f * pulse),
            (uint8_t)(160.f + 80.f * pulse),
            255, 220
        ));
        box.setOutlineThickness(2.f);
        window.draw(box);

        sf::Text titleTxt(font, title, 24);
        titleTxt.setFillColor(sf::Color(0, 220, 255));
        titleTxt.setStyle(sf::Text::Bold);
        sf::FloatRect tb = titleTxt.getLocalBounds();
        titleTxt.setOrigin({tb.size.x / 2.f, tb.size.y / 2.f});
        titleTxt.setPosition({W / 2.f, boxY + 42.f});
        window.draw(titleTxt);

        sf::RectangleShape div({boxW - 60.f, 1.f});
        div.setPosition({boxX + 30.f, boxY + 72.f});
        div.setFillColor(sf::Color(0, 180, 255, 80));
        window.draw(div);

        sf::Text bodyTxt(font, body, 17);
        bodyTxt.setFillColor(sf::Color(200, 200, 230));
        sf::FloatRect bb = bodyTxt.getLocalBounds();
        bodyTxt.setOrigin({bb.size.x / 2.f, bb.size.y / 2.f});
        bodyTxt.setPosition({W / 2.f, boxY + 115.f});
        window.draw(bodyTxt);

        sf::Text hint(font, "Press ENTER to continue", 14);
        hint.setFillColor(sf::Color(80, 80, 120));
        sf::FloatRect hb = hint.getLocalBounds();
        hint.setOrigin({hb.size.x / 2.f, hb.size.y / 2.f});
        hint.setPosition({W / 2.f, boxY + boxH - 24.f});
        window.draw(hint);

        window.display();
    }
}

void Game::pauseScreen() {
    // merged into showGUIMessage
}

Game::Game(sf::RenderWindow& win, sf::Font& fnt, const string& name)
    : player(1, 1), window(win), font(fnt), playerName(name) {
    gameOver = false;
    scores.push_back(50);
    scores.push_back(80);
    scores.push_back(30);
    setupMonstersForLevel();
}

Game::~Game() {}

void Game::showInstructions() {
    sf::Vector2u sz = window.getSize();
    float W = (float)sz.x;
    float H = (float)sz.y;

    sf::Clock clock;
    bool done = false;

    while (window.isOpen() && !done) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) { window.close(); return; }
            if (const auto* k = event->getIf<sf::Event::KeyPressed>()) {
                if (k->code == sf::Keyboard::Key::Enter ||
                    k->code == sf::Keyboard::Key::Space) done = true;
            }
        }

        float t = clock.getElapsedTime().asSeconds();
        float pulse = 0.5f + 0.5f * std::sin(t * 2.f);

        window.clear(sf::Color(10, 10, 22));

        for (int i = 0; i < 20; i++) {
            sf::RectangleShape line({W, 1.f});
            line.setPosition({0.f, i * (H / 20.f)});
            line.setFillColor(sf::Color(30, 30, 55, 50));
            window.draw(line);
        }

        float boxW = 520.f;
        float boxH = 420.f;
        float boxX = (W - boxW) / 2.f;
        float boxY = (H - boxH) / 2.f;

        sf::RectangleShape box({boxW, boxH});
        box.setPosition({boxX, boxY});
        box.setFillColor(sf::Color(14, 14, 30, 240));
        box.setOutlineColor(sf::Color(
            (uint8_t)(40.f * pulse),
            (uint8_t)(160.f + 80.f * pulse),
            255, 200
        ));
        box.setOutlineThickness(2.f);
        window.draw(box);

        sf::Text title(font, "HOW TO PLAY", 32);
        title.setFillColor(sf::Color(0, 210, 255));
        title.setStyle(sf::Text::Bold);
        sf::FloatRect tb = title.getLocalBounds();
        title.setOrigin({tb.size.x / 2.f, tb.size.y / 2.f});
        title.setPosition({W / 2.f, boxY + 45.f});
        window.draw(title);

        sf::RectangleShape divLine({boxW - 60.f, 1.f});
        divLine.setPosition({boxX + 30.f, boxY + 78.f});
        divLine.setFillColor(sf::Color(0, 180, 255, 80));
        window.draw(divLine);

        vector<string> lines = {
            "W / A / S / D   -   Move up / left / down / right",
            "U               -   Undo last move",
            "I               -   Show inventory",
            "Q               -   Quit game",
            "",
            "Collect the Treasure  T  then reach the Exit  E",
            "Avoid monsters  M  - they follow you!",
            "Each level adds more monsters. Good luck!"
        };

        float lineY = boxY + 105.f;
        for (const string& line : lines) {
            sf::Text txt(font, line, 17);
            txt.setFillColor(line.empty() ? sf::Color::Transparent : sf::Color(190, 190, 230));
            sf::FloatRect lb = txt.getLocalBounds();
            txt.setOrigin({lb.size.x / 2.f, 0.f});
            txt.setPosition({W / 2.f, lineY});
            window.draw(txt);
            lineY += 32.f;
        }

        sf::Text hint(font, "Press ENTER to Start", 18);
        hint.setFillColor(sf::Color(
            60,
            (uint8_t)(200.f + 55.f * pulse),
            100
        ));
        sf::FloatRect hb = hint.getLocalBounds();
        hint.setOrigin({hb.size.x / 2.f, hb.size.y / 2.f});
        hint.setPosition({W / 2.f, boxY + boxH - 35.f});
        window.draw(hint);

        window.display();
    }
}

void Game::start() {
    showInstructions();

    while (!gameOver && window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
            if (const auto* k = event->getIf<sf::Event::KeyPressed>()) {
                char input = 0;
                if (k->code == sf::Keyboard::Key::W) input = 'w';
                else if (k->code == sf::Keyboard::Key::A) input = 'a';
                else if (k->code == sf::Keyboard::Key::S) input = 's';
                else if (k->code == sf::Keyboard::Key::D) input = 'd';
                else if (k->code == sf::Keyboard::Key::U) input = 'u';
                else if (k->code == sf::Keyboard::Key::I) input = 'i';
                else if (k->code == sf::Keyboard::Key::Q) input = 'q';

                if (input != 0) {
                    try {
                        handleInput(input);
                    } catch (GameException& e) {
                        showGUIMessage("Oops!", e.what());
                    }
                }
            }
        }

        window.clear(sf::Color(10, 10, 22));
        dungeon.display(player, monsters, window, font, playerName);
        window.display();

        checkGameStatus();
    }

    if (window.isOpen()) endGame();
}

void Game::handleInput(char input) {
    if (input == 'q') { gameOver = true; return; }

    if (input == 'i') {
        string inv = player.hasTreasure() ? "Treasure" : "Empty";
        showGUIMessage("Inventory", "Items: " + inv);
        return;
    }

    if (input == 'u') { undoMove(); return; }

    int dx = 0, dy = 0;
    if      (input == 'w') dy = -1;
    else if (input == 's') dy =  1;
    else if (input == 'a') dx = -1;
    else if (input == 'd') dx =  1;
    else throw GameException("Invalid input. Use W A S D U I Q.");

    movePlayer(dx, dy);

    if (!handleMonsterAttack()) {
        moveMonstersTowardPlayer();
    }
}

void Game::movePlayer(int dx, int dy) {
    int newX = player.getX() + dx;
    int newY = player.getY() + dy;

    if (dungeon.isWall(newX, newY)) throw GameException("You hit a wall!");

    previousMoves.push({player.getX(), player.getY()});
    player.setPosition(newX, newY);

    char cell = dungeon.getCell(newX, newY);
    if (cell == 'T') {
        player.addItem("Treasure");
        extraInventory.add("Treasure");
        dungeon.setCell(newX, newY, '.');
        showGUIMessage("Treasure Found!", "You collected the treasure! Now find the exit E.");
    }
}

void Game::undoMove() {
    if (previousMoves.empty()) {
        showGUIMessage("Undo", "No move to undo.");
        return;
    }
    pair<int,int> old = previousMoves.top();
    previousMoves.pop();
    player.setPosition(old.first, old.second);
    showGUIMessage("Undo", "Last move undone.");
}

int Game::distanceToPlayer(int x, int y) {
    return abs(player.getX() - x) + abs(player.getY() - y);
}

bool Game::isMonsterPositionUsed(int x, int y, int idx) {
    for (int i = 0; i < (int)monsters.size(); i++) {
        if (i != idx && monsters[i].getX() == x && monsters[i].getY() == y)
            return true;
    }
    return false;
}

void Game::moveMonstersTowardPlayer() {
    for (int i = 0; i < (int)monsters.size(); i++) {
        int cx = monsters[i].getX();
        int cy = monsters[i].getY();
        int bx = cx, by = cy;
        int bd = distanceToPlayer(cx, cy);

        vector<pair<int,int>> moves = {{cx, cy-1},{cx, cy+1},{cx-1, cy},{cx+1, cy}};
        for (auto& mv : moves) {
            if (!dungeon.isWall(mv.first, mv.second) &&
                !isMonsterPositionUsed(mv.first, mv.second, i)) {
                int nd = distanceToPlayer(mv.first, mv.second);
                if (nd < bd) { bd = nd; bx = mv.first; by = mv.second; }
            }
        }
        monsters[i].setPosition(bx, by);
    }
}

bool Game::handleMonsterAttack() {
    for (int i = 0; i < (int)monsters.size(); i++) {
        if (player.getX() == monsters[i].getX() &&
            player.getY() == monsters[i].getY()) {
            player.setHealth(player.getHealth() - 10);
            monsters.erase(monsters.begin() + i);
            showGUIMessage("Monster Attack!", "A monster hit you! -10 HP. Monster defeated.");
            if (player.getHealth() <= 0) {
                showGUIMessage("You Died", "Game over. Better luck next time!");
                gameOver = true;
            }
            return true;
        }
    }
    return false;
}

void Game::setupMonstersForLevel() {
    monsters.clear();
    int count = dungeon.getCurrentLevelNumber();

    vector<pair<int,int>> spawns = {{5,3},{7,1},{2,5},{8,6},{10,9},{3,8}};
    for (auto& pos : spawns) {
        if ((int)monsters.size() >= count) break;
        int x = pos.first, y = pos.second;
        if (!dungeon.isWall(x,y) && dungeon.getCell(x,y) != 'T' &&
            dungeon.getCell(x,y) != 'E' && !(x == player.getX() && y == player.getY())) {
            monsters.push_back(Monster(x, y));
        }
    }
    for (int y = 1; y < dungeon.getHeight()-1 && (int)monsters.size() < count; y++) {
        for (int x = 1; x < dungeon.getWidth()-1 && (int)monsters.size() < count; x++) {
            if (!dungeon.isWall(x,y) && dungeon.getCell(x,y) != 'T' &&
                dungeon.getCell(x,y) != 'E' && !(x == player.getX() && y == player.getY())) {
                bool used = false;
                for (const Monster& m : monsters) if (m.getX()==x && m.getY()==y) { used=true; break; }
                if (!used) monsters.push_back(Monster(x,y));
            }
        }
    }
}

void Game::resetForNextLevel() {
    while (!previousMoves.empty()) previousMoves.pop();
    player.clearInventory();
    player.setPosition(1, 1);
    setupMonstersForLevel();
}

void Game::checkGameStatus() {
    handleMonsterAttack();
    if (gameOver) return;

    char cell = dungeon.getCell(player.getX(), player.getY());
    if (cell == 'E') {
        if (player.hasTreasure()) {
            if (dungeon.isLastLevel()) {
                scores.push_back(player.getHealth());
                showGUIMessage("Victory!", "You escaped the final dungeon! Congratulations, " + playerName + "!");
                gameOver = true;
            } else {
                showGUIMessage("Level Complete!", "Moving to the next level. Stay sharp!");
                dungeon.nextLevel();
                resetForNextLevel();
            }
        } else {
            showGUIMessage("Not Yet!", "You need to collect the Treasure T first!");
        }
    }
}

int Game::recursiveBonus(int n) {
    if (n <= 0) return 0;
    return 1 + recursiveBonus(n - 1);
}

void Game::endGame() {
    int bonus = recursiveBonus(5);
    int finalScore = player.getHealth() + bonus;
    scores.push_back(finalScore);

    sort(scores.begin(), scores.end(), greater<int>());

    // Lambda function - filters good scores
    auto goodScore = [](int score) {
        return score >= 50;
    };

    string scoreStr = "Final Score: " + to_string(finalScore) +
                      "  (+" + to_string(bonus) + " bonus)\n";

    scoreStr += "Leaderboard: ";
    for (int s : scores) scoreStr += to_string(s) + " ";

    scoreStr += "\nGood Scores (>=50): ";
    for (int s : scores) {
        if (goodScore(s)) scoreStr += to_string(s) + " ";
    }

    showGUIMessage("Game Over - " + playerName, scoreStr);
}

void showProjectInfo(Game& game) {
    cout << "Project uses: classes, inheritance, polymorphism, abstract classes,\n"
         << "stacks, vectors, exceptions, templates, recursion, lambdas, sorting,\n"
         << "constructors, destructors, copy constructor, operator overloading,\n"
         << "friend functions, and SFML GUI.\n";
}