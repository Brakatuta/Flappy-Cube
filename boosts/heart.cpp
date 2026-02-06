#include "heart.h"
#include <GL/freeglut.h>

ModelOBJ* Heart::heartModel = nullptr;

Heart::Heart(float startX, float startY) : Boost(startX, startY) {}

void Heart::setModel(ModelOBJ* model) { heartModel = model; }

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