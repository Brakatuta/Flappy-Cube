#ifndef PIPE_H
#define PIPE_H

#pragma once

class Pipe {
public:
    Pipe(float startX, float gapY);

    // Updates position and returns true if it should be deleted (off-screen)
    bool update(float worldSpeed);
    
    // Renders the top and bottom pipe
    void draw(float currentGap) const;

    // Logic checks
    bool checkCollision(float cubeY, float cubeSize, float currentGap) const;
    bool hasBeenPassed(float playerX) const;
    void setPassed(bool status) { passed = status; }

    float getX() const { return x; }
    float getGapY() const { return gapY; }

private:
    float x;
    float gapY;
    bool passed;
};

#endif