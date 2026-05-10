#include "Monster.h"

Monster::Monster(int x, int y) : Entity(x, y) {}

char Monster::getSymbol() const { return 'M'; }