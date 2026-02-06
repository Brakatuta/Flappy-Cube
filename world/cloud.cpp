#include "cloud.h"
#include "../utils/draw.h"
#include <GL/freeglut.h>

Cloud::Cloud(float x, float y, float z, float w, float h, float d, float speedMult) 
    : x(x), y(y), z(z), w(w), h(h), d(d), speedMult(speedMult) {}

bool Cloud::update(float worldSpeed) {
    x -= worldSpeed * speedMult;
    return x < -20.0f;
}

void Cloud::draw() const {
    glPushMatrix();
        glTranslatef(x, y, z);
        glColor3f(0.95f, 0.95f, 1.0f);

        // Draw main body
        drawBox(w, h, d);

        // Draw "puffs" relative to main body to create volume
        glPushMatrix();
            glTranslatef(w * 0.2f, h * 0.3f, 0.1f);
            drawBox(w * 0.6f, h * 0.8f, d);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-w * 0.25f, -h * 0.2f, -0.1f);
            drawBox(w * 0.5f, h * 0.6f, d);
        glPopMatrix();
    glPopMatrix();
}