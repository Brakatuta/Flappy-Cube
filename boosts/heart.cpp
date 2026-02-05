#include "heart.h"
#include <GL/freeglut.h>
#include <cmath>

ModelOBJ* Heart::heartModel = nullptr;

Heart::Heart(float startX, float startY)
    : x(startX), y(startY), rotation(0.0f), collected(false) {}

void Heart::setModel(ModelOBJ* model) {
    heartModel = model;
}

void Heart::update(float worldSpeed, float playerX, float playerY) {
    if (collected) return;

    x -= worldSpeed;
    rotation += 5.0f;

    float dx = x - playerX;
    float dy = y - playerY;
    float distSq = dx * dx + dy * dy;

    if (distSq < 0.36f) {
        collected = true;
    }
}

void Heart::draw() const {
    if (collected || !heartModel) return;

    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glRotatef(rotation, 0, 1, 0);
        glScalef(0.2f, 0.2f, 0.2f);
        glColor3f(1.0f, 0.2f, 0.2f);
        heartModel->draw();
    glPopMatrix();
}

bool Heart::isCollected() const {
    return collected;
}

float Heart::getX() const {
    return x;
}
