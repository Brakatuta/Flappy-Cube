#ifndef STAR_H
#define STAR_H

#pragma once

#include "../utils/ModelOBJ.h"


class Star {
public:
    Star(float startX, float startY);

    void update(float worldSpeed, float playerX, float playerY);
    void draw() const;

    bool isCollected() const;
    float getX() const;

    // Call once at startup
    static void setModel(ModelOBJ* model);

private:
    float x, y;
    float rotation;
    bool collected;

    static ModelOBJ* starModel;
};

#endif
