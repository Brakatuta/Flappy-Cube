#include "star.h"
#include <GL/freeglut.h>
#include <cmath>

ModelOBJ* Star::starModel = nullptr;

Star::Star(float startX, float startY) : Boost(startX, startY) {}

void Star::setModel(ModelOBJ* model) { starModel = model; }

void Star::draw() const {
    if (collected || !starModel) return;

    float t = glutGet(GLUT_ELAPSED_TIME) * 0.005f; 
    float r = 0.5f + 0.5f * sin(t); // Unique rainbow logic
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