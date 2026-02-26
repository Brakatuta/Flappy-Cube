#include "../game.h"
#include "gui.h"
#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>

void myOrtho2D(float left, float right, float bottom, float top)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void drawUIPanel(float x, float y, float w, float h) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
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

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void drawUI() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    myOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    float pulse = 0.7f + 0.3f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.005f);
    char buf[64]; // Puffer für Text-Formatierung

    // Lambdas jetzt mit const char* statt string
    auto drawT = [](float x, float y, const char* t) {
        glRasterPos2f(x, y);
        for (const char* c = t; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    };

    auto drawBigT = [](float x, float y, const char* t) {
        glRasterPos2f(x, y);
        for (const char* c = t; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    };

    auto drawSmallT = [](float x, float y, const char* t) {
        glRasterPos2f(x, y);
        for (const char* c = t; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    };

    if (currentState == MENU) {
        drawUIPanel(280, 260, 240, 200);

        sprintf(buf, "HIGHSCORE: %d", highScore);
        glColor3f(0, 1, 0);
        drawT(285, 435, buf);

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
        
        sprintf(buf, "Highscore: %d", highScore);
        drawT(20, 565, buf);
        
        sprintf(buf, "Score: %d", score);
        drawT(20, 540, buf);
        
        sprintf(buf, "Lives: %d", extraLives + 1);
        drawT(20, 515, buf);
    }
    else if (currentState == GAMEOVER) {
        if (score > highScore) {
            highScore = score;
            printf("New High Score: %d\n", highScore);
            saveHighScore();
        }

        drawUIPanel(280, 250, 240, 180);
        glColor3f(1, 0, 0); drawT(365, 390, "CRASHED!");
        
        sprintf(buf, "SCORE: %d", score);
        glColor3f(1, 1, 1); drawT(370, 350, buf);
        
        sprintf(buf, "HIGHSCORE: %d", highScore);
        glColor3f(0, 1, 0); drawT(355, 325, buf);

        glColor3f(pulse, pulse, pulse); drawT(330, 290, "PRESS 'S' TO RESTART");
        glColor3f(1, 1, 1); drawT(320, 270, "PRESS 'M' TO GO TO MENU");
    }
    else if (currentState == PAUSED) {
        drawUIPanel(280, 260, 240, 100);
        glColor3f(1, 1, 1);
        drawT(360, 320, "GAME PAUSED");

        glColor3f(pulse, pulse, pulse);
        drawT(335, 290, "PRESS 'P' TO RESUME");
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
}