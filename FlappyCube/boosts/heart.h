#ifndef HEART_H
#define HEART_H

#pragma once

#include "../utils/ModelOBJ.h"


class Heart {
public:
    Heart(float startX, float startY);

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

    static ModelOBJ* heartModel;
};

#endif
