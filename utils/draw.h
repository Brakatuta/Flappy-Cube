#ifndef DRAW_H
#define DRAW_H

#pragma once

#include <vector>

// Mountains Data
struct Mountain {
    float x, y, z;
    float w, h;
    float r, g, b;
    float speedMult;
};

// Cloud Data
struct Cloud {
    float x, y, z;
    float w, h, d;
    float speedMult;
};

// Pipe Data
struct Pipe {
    float x;
    float gapY;
    bool passed;
};



void drawBox(float width, float height, float depth);

void drawMountain(const Mountain& m);
void drawCloud(Cloud c);
void drawGround(double floorOffset);
void drawPipes(std::vector<Pipe> pipes, double currentGap);

#endif
