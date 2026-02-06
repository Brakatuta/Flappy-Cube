#ifndef COIN_H
#define COIN_H

#include "Boost.h"

class Coin : public Boost {
public:
    Coin(float startX, float startY);
    void draw() const override; // Overriding the base draw
};

#endif