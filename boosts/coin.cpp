#include "coin.h"
#include <GL/freeglut.h>

void drawBox(float width, float height, float depth);

Coin::Coin(float startX, float startY) : Boost(startX, startY) {}

void Coin::draw() const {
    if (collected) return; // Accessing protected 'collected' from Boost

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