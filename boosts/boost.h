#ifndef BOOST_H
#define BOOST_H

#pragma once

#include <cmath>

class Boost {
public:
    Boost(float startX, float startY) 
        : x(startX), y(startY), rotation(0.0f), collected(false) {}

    // Virtual destructor need for base class
    virtual ~Boost() {}

    // Common update logic for all boosts
    virtual void update(float worldSpeed, float playerX, float playerY) {
        if (collected) return;

        x -= worldSpeed;
        rotation += 5.0f;

        // Standard collision check (0.6 distance threshold)
        float dx = x - playerX;
        float dy = y - playerY;
        float distSq = dx * dx + dy * dy;

        if (distSq < 0.36f) {
            collected = true;
        }
    }

    // Pure virtual: every boost must define how it looks
    virtual void draw() const = 0;

    bool isCollected() const { return collected; }
    float getX() const { return x; }
    float getY() const { return y; }

protected:
    float x, y;
    float rotation;
    bool collected;
};

#endif