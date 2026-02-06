#ifndef MOUNTAIN_H
#define MOUNTAIN_H

#pragma once

class Mountain {
public:
    Mountain(float x, float y, float z, float w, float h, float r, float g, float b, float speedMulti);

    bool update(float currentSpeed);
    void draw() const;

private:
    float x, y, z;
    float w, h;
    float r, g, b;
    float speedMult;
};

#endif