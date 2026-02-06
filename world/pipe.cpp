#include "pipe.h"
#include "../utils/draw.h"
#include <GL/freeglut.h>

Pipe::Pipe(float startX, float gapY) 
    : x(startX), gapY(gapY), passed(false) {}

bool Pipe::update(float worldSpeed) {
    x -= worldSpeed;
    return (x < -10.0f); // Return true if off-screen
}

void Pipe::draw(float currentGap) const {
    glPushMatrix();
    glTranslatef(x, 0, 0);

    // Bottom Pipe
    glPushMatrix();
        glColor3f(0.0f, 0.5f, 0.2f);
        glTranslatef(0, gapY - currentGap - 4.0f, 0);
        drawBox(0.8f, 8.0f, 0.8f); // Main shaft
    glPopMatrix();

    // Bottom Rim
    glPushMatrix();
        glColor3f(0.0f, 0.6f, 0.2f);
        glTranslatef(0, gapY - currentGap - 0.1f, 0);
        drawBox(1.0f, 0.3f, 1.0f);
    glPopMatrix();


    // Top Pipe
    glPushMatrix();
        glColor3f(0.0f, 0.5f, 0.2f);
        glTranslatef(0, gapY + currentGap + 4.0f, 0);
        drawBox(0.8f, 8.0f, 0.8f); // Main shaft
    glPopMatrix();

    // Top Rim
    glPushMatrix();
        glColor3f(0.0f, 0.6f, 0.2f);
        glTranslatef(0, gapY + currentGap + 0.1f, 0);
        drawBox(1.0f, 0.3f, 1.0f);
    glPopMatrix();

    glPopMatrix();
}

bool Pipe::checkCollision(float cubeY, float cubeSize, float currentGap) const {
    float cubeLeft = -1.5f - (cubeSize / 2);
    float cubeRight = -1.5f + (cubeSize / 2);
    float cubeTop = cubeY + (cubeSize / 2);
    float cubeBottom = cubeY - (cubeSize / 2);
    
    float pipeLeft = x - 0.4f;
    float pipeRight = x + 0.4f;

    // Horizontal overlap
    if (cubeRight > pipeLeft && cubeLeft < pipeRight) {
        // Vertical gap check
        if (cubeTop > gapY + currentGap || cubeBottom < gapY - currentGap) {
            return true;
        }
    }
    return false;
}

bool Pipe::hasBeenPassed(float playerX) const {
    return (!passed && x < playerX);
}