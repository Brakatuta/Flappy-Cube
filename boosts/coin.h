#ifndef COIN_H
#define COIN_H

#pragma once


class Coin {
public:
    Coin(float startX, float startY);

    void update(float worldSpeed, float playerX, float playerY);
    void draw() const;

    bool isCollected() const;
    float getX() const;

private:
    float x, y;
    float rotation;
    bool collected;
};

#endif
