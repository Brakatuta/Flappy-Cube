#include "star.h"
#include <GL/freeglut.h>
#include <cmath>

ModelOBJ* Star::starModel = nullptr;

Star::Star(float startX, float startY)
    : x(startX), y(startY), rotation(0.0f), collected(false) {}

void Star::setModel(ModelOBJ* model) {
    starModel = model;
}

void Star::update(float worldSpeed, float playerX, float playerY) {
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

void Star::draw() const {
    if (collected || !starModel) return;

    float t = glutGet(GLUT_ELAPSED_TIME) * 0.005f; // seconds-ish

    // Rainbow using phase-shifted sine waves
    float r = 0.5f + 0.5f * sin(t);
    float g = 0.5f + 0.5f * sin(t + 2.0f);
    float b = 0.5f + 0.5f * sin(t + 4.0f);

    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glRotatef(rotation, 0, 1, 0);
        glScalef(0.015f, 0.015f, 0.015f);

        glDisable(GL_LIGHTING);
        glColor3f(r, g, b);
        starModel->draw();
        glEnable(GL_LIGHTING);
    glPopMatrix();
}

bool Star::isCollected() const {
    return collected;
}

float Star::getX() const {
    return x;
}
