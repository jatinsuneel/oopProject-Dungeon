#ifndef MONSTER_H
#define MONSTER_H

#include "Entity.h"

class Monster : public Entity {
public:
    Monster(int x = 5, int y = 3);
    char getSymbol() const override;
};

#endif