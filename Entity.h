#ifndef ENTITY_H
#define ENTITY_H

#include <string>
using namespace std;

class Entity {
protected:
    int x;
    int y;
    string name;

public:
    Entity(int x = 0, int y = 0);

    int getX() const;
    int getY() const;

    void setPosition(int x, int y);

    void setName(string n);
    string getName() const;

    virtual char getSymbol() const = 0;

    virtual ~Entity();
};

#endif