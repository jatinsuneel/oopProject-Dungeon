#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <vector>
#include <string>

using namespace std;

class Player : public Entity {
private:
    int health;
    vector<string> inventory;

public:
    Player(int x = 1, int y = 1);
    Player(const Player& other);

    bool operator==(const Player& other) const;

    int getHealth() const;
    void setHealth(int health);

    void addItem(string item);
    bool hasTreasure() const;
    void clearInventory();
    void showInventory() const;

    char getSymbol() const override;
};

#endif