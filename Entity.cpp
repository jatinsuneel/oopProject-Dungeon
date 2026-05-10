#include "Entity.h"

Entity::Entity(int x, int y) {
    this->x = x;
    this->y = y;
    this->name = "";
}

int Entity::getX() const { return x; }
int Entity::getY() const { return y; }

void Entity::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void Entity::setName(string n) { this->name = n; }
string Entity::getName() const { return name; }

Entity::~Entity() {}