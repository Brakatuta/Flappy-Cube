#include "mountain.h"
#include "../utils/draw.h"
#include <GL/freeglut.h>

Mountain::Mountain(float x, float y, float z, float w, float h, float r, float g, float b, float speedMult) 
    : x(x), y(y), z(z), w(w), h(h), r(r), g(g), b(b), speedMult(speedMult) {}

bool Mountain::update(float worldSpeed) {
    x -= worldSpeed * speedMult;
    return x < -30.0f;
}

void Mountain::draw() const {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(45, 0, 0, 1);

    // Apply lighting/color
    float mat_emission[] = { r * 0.3f, g * 0.3f, b * 0.3f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glColor3f(r, g, b);

    // Use stored width (m.w) and height (m.h) for variation
    drawBox(w, h, 8.0f);

    float no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    glPopMatrix();
}