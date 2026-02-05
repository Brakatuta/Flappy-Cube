#include "Coin.h"
#include <GL/freeglut.h>
#include <cmath>

// already have drawBox(), so just declare it
void drawBox(float width, float height, float depth);

Coin::Coin(float startX, float startY)
    : x(startX), y(startY), rotation(0.0f), collected(false) {}

void Coin::update(float worldSpeed, float playerX, float playerY) {
    if (collected) return;

    // Move with world
    x -= worldSpeed;
    rotation += 5.0f;

    // Collision check
    float dx = x - playerX;
    float dy = y - playerY;
    float distSq = dx * dx + dy * dy;

    if (distSq < 0.36f) {
        collected = true;
    }
}

void Coin::draw() const {
    if (collected) return;

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(rotation, 0, 1, 0);

    glColor3f(1.0f, 0.84f, 0.0f);
    drawBox(0.4f, 0.4f, 0.1f);

    glDisable(GL_LIGHTING);
    glColor3f(0.88f, 0.64f, 0.0f);
    drawBox(0.2f, 0.2f, 0.11f);
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

bool Coin::isCollected() const {
    return collected;
}

float Coin::getX() const {
    return x;
}
