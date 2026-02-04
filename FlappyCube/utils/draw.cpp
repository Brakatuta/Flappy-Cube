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

void drawMountain(const Mountain& m) {
    glPushMatrix();
    glTranslatef(m.x, m.y, m.z);
    glRotatef(45, 0, 0, 1);

    // Apply lighting/color
    float mat_emission[] = { m.r * 0.3f, m.g * 0.3f, m.b * 0.3f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glColor3f(m.r, m.g, m.b);

    // Use stored width (m.w) and height (m.h) for variation
    drawBox(m.w, m.h, 8.0f);

    float no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    glPopMatrix();
}

void drawCloud(Cloud c) {
    glPushMatrix();
        glTranslatef(c.x, c.y, c.z);
        glColor3f(0.95f, 0.95f, 1.0f);

        // Draw main body
        drawBox(c.w, c.h, c.d);

        // Draw "puffs" relative to main body to create volume
        glPushMatrix();
            glTranslatef(c.w * 0.2f, c.h * 0.3f, 0.1f);
            drawBox(c.w * 0.6f, c.h * 0.8f, c.d);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-c.w * 0.25f, -c.h * 0.2f, -0.1f);
            drawBox(c.w * 0.5f, c.h * 0.6f, c.d);
        glPopMatrix();
    glPopMatrix();
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

void drawPipes(std::vector<Pipe> pipes, double currentGap) {
    for (auto& p : pipes) {
        glPushMatrix();
        glTranslatef(p.x, 0, 0);

        // --- BOTTOM PIPE ---
        glPushMatrix();
            glColor3f(0.0f, 0.5f, 0.2f);
            glTranslatef(0, p.gapY - currentGap - 4.0f, 0);
            drawBox(0.8f, 8.0f, 0.8f); // Main shaft
        glPopMatrix();

        // BOTTOM RIM (The "Lip")
        glPushMatrix();
            glColor3f(0.0f, 0.6f, 0.2f); // Slightly lighter green
            glTranslatef(0, p.gapY - currentGap - 0.1f, 0);
            drawBox(1.0f, 0.3f, 1.0f); // Wider (1.0f) and short (0.3f)
        glPopMatrix();


        // --- TOP PIPE ---
        glPushMatrix();
            glColor3f(0.0f, 0.5f, 0.2f);
            glTranslatef(0, p.gapY + currentGap + 4.0f, 0);
            drawBox(0.8f, 8.0f, 0.8f); // Main shaft
        glPopMatrix();

        // TOP RIM (The "Lip")
        glPushMatrix();
            glColor3f(0.0f, 0.6f, 0.2f);
            glTranslatef(0, p.gapY + currentGap + 0.1f, 0);
            drawBox(1.0f, 0.3f, 1.0f); // Wider (1.0f) and short (0.3f)
        glPopMatrix();

        glPopMatrix();
    }
}
