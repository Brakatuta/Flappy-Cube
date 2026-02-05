#include "../game.h"

#include "gui.h"
#include <GL/freeglut.h>
#include <cmath>
#include <string>

using namespace std;


void drawUIPanel(float x, float y, float w, float h) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST); // Crucial: Stop 3D objects from hiding the UI
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Background
    glColor4f(0.1f, 0.1f, 0.1f, 0.7f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();

    // Border
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();

    glEnable(GL_DEPTH_TEST); // Turn it back on for the next frame's 3D pass
    glDisable(GL_BLEND);
}

void drawMenuBackground() {
    if (menuBackground == 0) return;

    // --- RESET OPENGL STATES ---
    glDisable(GL_LIGHTING);   // If this is ON, textures look white/grey
    glDisable(GL_FOG);        // Fog often bleaches 2D quads to white
    glDisable(GL_DEPTH_TEST); // Ensures background is always "behind"
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, menuBackground);

    // Set color to pure white (so the texture isn't tinted/darkened)
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // --- SETUP 2D VIEW ---
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1280, 0, 720); // Your window resolution

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // --- DRAW THE QUAD ---
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(0, 0);       // Bottom Left
        glTexCoord2f(1, 1); glVertex2f(1280, 0);    // Bottom Right
        glTexCoord2f(1, 0); glVertex2f(1280, 720);  // Top Right
        glTexCoord2f(0, 0); glVertex2f(0, 720);     // Top Left
    glEnd();

    // --- RESTORE STATES ---
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
}

void drawUI() {
    // Switch to Projection mode to set up 2D coordinates
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    // Switch to ModelView and RESET IT
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    float pulse = 0.7f + 0.3f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.005f);

    auto drawT = [](float x, float y, string t) {
        glRasterPos2f(x, y);
        for (char c : t) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    };

    auto drawBigT = [](float x, float y, string t) {
        glRasterPos2f(x, y);
        for (char c : t) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    };

    // Helper for small control text
    auto drawSmallT = [](float x, float y, string t) {
        glRasterPos2f(x, y);
        for (char c : t) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    };

    if (currentState == MENU) {
        drawUIPanel(280, 260, 240, 200);

        string hsText = "HIGHSCORE: " + to_string(highScore);
        glColor3f(0, 1, 0);
        drawT(285, 435, hsText);

        glColor3f(219 / 255.0, 153 / 255.0, 29 / 255.0);
        drawBigT(348, 360, "FLAPPY CUBE");

        glColor3f(pulse, pulse, pulse);
        drawT(340, 300, "PRESS 'S' TO START");

        drawUIPanel(10, 485, 100, 100);

        glColor3f(1, 1, 1);
        drawT(20, 565, "Controls");

        glBegin(GL_LINES);
            glVertex2f(20, 560);
            glVertex2f(100, 560);
        glEnd();

        drawSmallT(20, 540, "S: Start / Restart");
        drawSmallT(20, 525, "P: Pause");
        drawSmallT(20, 510, "M: Go to Menu");
        drawSmallT(20, 495, "Space: Jump");
    }
    else if (currentState == PLAYING) {
        drawUIPanel(10, 505, 110, 80);
        glColor3f(1, 1, 1);
        drawT(20, 565, "Highscore: " + to_string(highScore));
        drawT(20, 540, "Score: " + to_string(score));
        drawT(20, 515, "Lives: " + to_string(extraLives + 1));
    }
    else if (currentState == GAMEOVER) {
        if (score > highScore) {
            highScore = score;
            saveHighScore();
        }

        drawUIPanel(280, 250, 240, 180);
        glColor3f(1, 0, 0); drawT(365, 390, "CRASHED!");
        glColor3f(1, 1, 1); drawT(370, 350, "SCORE: " + to_string(score));
        glColor3f(0, 1, 0); drawT(355, 325, "HIGHSCORE: " + to_string(highScore));

        glColor3f(pulse, pulse, pulse); drawT(330, 290, "PRESS 'S' TO RESTART");
        glColor3f(1, 1, 1); drawT(320, 270, "PRESS 'M' TO GO TO MENU");
    }
    else if (currentState == PAUSED) {
        drawUIPanel(280, 260, 240, 100);
        glColor3f(1, 1, 1);
        drawT(360, 320, "GAME PAUSED");

        float pulse = 0.7f + 0.3f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.005f);
        glColor3f(pulse, pulse, pulse);
        drawT(335, 290, "PRESS 'P' TO RESUME");
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
}
