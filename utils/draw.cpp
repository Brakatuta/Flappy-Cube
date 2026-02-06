#include "draw.h"
#include <GL/freeglut.h>
#include <vector>


// Helper function
void drawBox(float width, float height, float depth) {
    float w = width / 2.0f, h = height / 2.0f, d = depth / 2.0f;
    glBegin(GL_QUADS);

    // Front Face (Pointing toward +Z)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-w, -h,  d); glVertex3f( w, -h,  d); glVertex3f( w,  h,  d); glVertex3f(-w,  h,  d);

    // Back Face (Pointing toward -Z)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-w, -h, -d); glVertex3f(-w,  h, -d); glVertex3f( w,  h, -d); glVertex3f( w, -h, -d);

    // Top Face (Pointing toward +Y)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-w,  h, -d); glVertex3f(-w,  h,  d); glVertex3f( w,  h,  d); glVertex3f( w,  h, -d);

    // Bottom Face (Pointing toward -Y)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-w, -h, -d); glVertex3f( w, -h, -d); glVertex3f( w, -h,  d); glVertex3f(-w, -h,  d);

    // Left Face (Pointing toward -X)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-w, -h, -d); glVertex3f(-w, -h,  d); glVertex3f(-w,  h,  d); glVertex3f(-w,  h, -d);

    // Right Face (Pointing toward +X)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f( w, -h, -d); glVertex3f( w,  h, -d); glVertex3f( w,  h,  d); glVertex3f( w, -h,  d);

    glEnd();
}

void drawGround(double floorOffset) {
    glPushMatrix();
    glTranslatef(0, -3.0f, 0);
    for (int i = -12; i < 12; i++) {
        glPushMatrix();
        glTranslatef(i * 2.5f + floorOffset, 0, 0);
        glColor3f(0.1f, 0.3f, 0.1f);
        drawBox(2.75f, 0.2f, 10.0f);
        glPopMatrix();
    }
    glPopMatrix();
}