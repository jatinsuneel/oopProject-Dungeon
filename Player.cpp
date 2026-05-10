#include "Player.h"
#include <iostream>
#include <algorithm>

using namespace std;

Player::Player(int x, int y) : Entity(x, y) {
    health = 100;
}

Player::Player(const Player& other) : Entity(other.x, other.y) {
    health = other.health;
    inventory = other.inventory;
}

bool Player::operator==(const Player& other) const {
    return x == other.x && y == other.y && health == other.health;
}

int Player::getHealth() const { return health; }

void Player::setHealth(int health) {
    this->health = health;
    if (this->health < 0) this->health = 0;
}

void Player::addItem(string item) {
    inventory.push_back(item);
}

bool Player::hasTreasure() const {
    return find(inventory.begin(), inventory.end(), "Treasure") != inventory.end();
}

void Player::clearInventory() {
    inventory.clear();
}

void Player::showInventory() const {
    cout << "Inventory: ";
    if (inventory.empty()) {
        cout << "Empty";
    } else {
        for (string item : inventory) cout << item << " ";
    }
    cout << endl;
}

char Player::getSymbol() const { return 'P'; }