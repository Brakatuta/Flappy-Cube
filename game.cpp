#include <windows.h>
#include <GL/freeglut.h>
#include <GL/glext.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <math.h>
#include <algorithm>

#include "utils/draw.h"
#include "utils/gui.h"
#include "utils/ModelOBJ.h"
#include "utils/image.h"
#include "world/pipe.h"
#include "world/mountain.h"
#include "world/cloud.h"
#include "boosts/coin.h"
#include "boosts/heart.h"
#include "boosts/star.h"

using namespace std;

enum State { MENU, PLAYING, GAMEOVER, PAUSED };
State currentState = MENU;

// Physics
float cubeY = 0.0f, velocity = 0.0f;
const float gravity = -0.0028f;
const float jumpForce = 0.055f;
const float cubeSize = 0.4f;

// Objects
vector<Cloud> clouds;
float cloudSpawnTimer = 0.0f;

vector<Mountain> mountains;

vector<Pipe> pipes;
float spawnTimer = 0.0f;
float spawnInterval = 3.5f;

// Coins data
vector<Coin> activeCoins;

// Hearts data
vector<Heart> activeHearts;
ModelOBJ heartModel;

// Stars data
vector<Star> activeStars;
ModelOBJ starModel;

// Player model
ModelOBJ playerModel;

int score = 0, highScore = 0;
int extraLives = 1;

float invincibilityTimer = 0.0f;
float superModeTimer = 0.0f;

float currentSpeed = 0.06f;
float currentGap = 0.9f;
float floorOffset = 0.0f;
float lastGapY = 0.0f;

GLuint menuBackground;


void saveHighScore() {
    ofstream outFile("highscore.txt");
    if (outFile.is_open()) {
        outFile << highScore;
        outFile.close();
        SetFileAttributesA("highscore.txt", FILE_ATTRIBUTE_HIDDEN);
    }
}

void loadHighScore() {
    ifstream inFile("highscore.txt");
    if (inFile.is_open()) {
        inFile >> highScore;
        inFile.close();
    }
}

bool checkCollision(float px, float py, float pGapY) {
    float cubeLeft = -1.5f - (cubeSize / 2);
    float cubeRight = -1.5f + (cubeSize / 2);
    float cubeTop = cubeY + (cubeSize / 2);
    float cubeBottom = cubeY - (cubeSize / 2);
    float pipeLeft = px - 0.4f;
    float pipeRight = px + 0.4f;

    if (cubeRight > pipeLeft && cubeLeft < pipeRight) {
        if (cubeTop > pGapY + currentGap || cubeBottom < pGapY - currentGap) return true;
    }
    return false;
}

void resetGame() {
    cubeY = 0.0f; velocity = 0.0f;
    pipes.clear();
    clouds.clear();
    extraLives = 0;
    activeHearts.clear();
    activeCoins.clear();
    activeStars.clear();
    lastGapY = 0.0f;
    pipes.push_back(Pipe(6.0f, 0.0f));
    spawnTimer = 0.0f;
    score = 0;
    currentSpeed = 0.06f;
    currentGap = 0.9f;
    spawnInterval = 3.5f;

    mountains.clear();

    // Pre-spawn mountains across the screen initially with the NEW struct format
    for(float i = -30.0f; i < 25.0f; i += 8.0f) {
        // Layer 1: Snowy (x, y, z, w, h, r, g, b, speed)
        float h1 = 15.0f;
        mountains.push_back(Mountain(i, -14.0f + (h1/2), -45.0f, h1, h1 * 1.5f, 0.8f, 0.85f, 0.9f, 0.05f));
        // Layer 2: Grey
        float h2 = 10.0f;
        mountains.push_back(Mountain(i + 4.0f, -9.0f + (h2/2), -30.0f, h2, h2 * 1.3f, 0.4f, 0.42f, 0.45f, 0.12f));
        // Layer 3: Green
        float h3 = 6.0f;
        mountains.push_back(Mountain(i + 2.0f, -8.0f + (h3/2), -15.0f, h3, h3 * 1.2f, 0.15f, 0.35f, 0.15f, 0.25f));
    }

    currentState = PLAYING;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (currentState == MENU) {
        drawMenuBackground();
    }

    if (currentState == PLAYING || currentState == PAUSED || currentState == GAMEOVER) {
        // camera position
        glTranslatef(0, 0, -7.0f);
        // camera tilt
        glRotatef(15, 1, 0, 0);
        // camera rotation
        glRotatef(-10, 0, 1, 0);

        // Draw Mountains
        for (auto& m : mountains) {
            m.draw();
        }

        // Draw Volumetric Clouds
        for (auto& c : clouds) {
            c.draw();
        }

        // Ground
        drawGround(floorOffset);

        // Pipes
        for (const auto& p : pipes) {
            p.draw(currentGap);
        }

        // Draw Coins
        for (const auto& c : activeCoins) {
            c.draw();
        }

        // Draw Hearts
        for (const auto& h : activeHearts) {
            h.draw();
        }

        // Draw Stars
        for (const auto& s : activeStars) {
            s.draw();
        }

        // Draw Player
        glPushMatrix();
            glTranslatef(-1.5f, cubeY, 0);
            glRotatef(velocity * 500.0f, 0, 0, 1);

            bool blink = ((int)(invincibilityTimer / 0.1f) % 2 == 0);

            if (!(invincibilityTimer > 0 && !blink)) {
                if (superModeTimer > 0) {
                    float t = glutGet(GLUT_ELAPSED_TIME) * 0.005f;
                    glColor3f(0.5f + 0.5f * sin(t), 0.5f + 0.5f * sin(t + 2.0f), 0.5f + 0.5f * sin(t + 4.0f));
                } else {
                    glColor3f(92/255.0f, 62/255.0f, 14/255.0f);
                }

                playerModel.setRotation(0, 90, 0);
                playerModel.setScale(0.225f);
                playerModel.draw();
            }
        glPopMatrix();
    }

    drawUI();
    glutSwapBuffers();
}

template <typename collectableType, typename OnCollectFn>

void updateCollectibles(vector<collectableType>& items, OnCollectFn onCollect) {
    for (auto item = items.begin(); item != items.end(); ) {
        item->update(currentSpeed, -1.5f, cubeY);

        if (item->isCollected()) {
            onCollect();
            item = items.erase(item);
        }
        else if (item->getX() < -10.0f) {
            item = items.erase(item);
        }
        else {
            ++item;
        }
    }
}

void updateMountains() {
    static float mountainSpawnTimer = 0.0f;
    mountainSpawnTimer += 0.016f;

    if (mountainSpawnTimer >= 0.8f) { // Frequent checks for variation
        int layer = rand() % 2;

        // Distant Snowy Peaks
        if (layer == 0) {
            float h = 11.0f + (rand() % 5);
            mountains.push_back({ 25.0f, -14.0f + (h/2), -45.0f, h, h * 1.5f, 0.8f, 0.85f, 0.9f, 0.05f });
        }

        // Mid-range Grey
        else if (layer == 1) {
            float h = 7.0f + (rand() % 3);
            mountains.push_back({ 25.0f, -9.0f + (h/2), -30.0f, h, h * 1.3f, 0.4f, 0.42f, 0.45f, 0.12f });
        }

        // Green Foothills
        if (rand() % 2 == 0) {
            float h = 4.0f + (rand() % 3);
            mountains.push_back({ 25.0f, -8.0f + (h/2), -15.0f, h, h * 1.2f, 0.15f, 0.35f, 0.15f, 0.25f });
        }

        mountainSpawnTimer = 0.0f;
    }

    for (auto it = mountains.begin(); it != mountains.end(); ) {
        if (it->update(currentSpeed)) { 
            it = mountains.erase(it);
        } else {
            ++it;
        }
    }
}

void updateClouds() {
    for (auto it = clouds.begin(); it != clouds.end(); ) {
        if (it->update(currentSpeed)) { 
            it = clouds.erase(it);
        } else {
            ++it;
        }
    }

    cloudSpawnTimer += 0.016f;
    if (cloudSpawnTimer >= 1.5f) {
        float cz = -12.0f - (rand() % 80 / 10.0f); // Depth variance
        float cy = 1.5f + (rand() % 25 / 10.0f);   // Height variance
        float cw = 1.5f + (rand() % 20 / 10.0f);   // Random Width
        float ch = 0.6f + (rand() % 10 / 10.0f);   // Random Height
        float speed = 0.15f + (rand() % 20 / 100.0f); // Variable parallax

        clouds.push_back(Cloud(18.0f, cy, cz, cw, ch, 0.8f, speed));
        cloudSpawnTimer = 0.0f;
    }
}

void update(int value) {
    if (currentState == PLAYING) {
        if (invincibilityTimer > 0) {
            invincibilityTimer -= 0.016f; // Decrease by frame time
        }

        if (superModeTimer > 0) {
            superModeTimer -= 0.016f;
            currentGap = 1.5f;
            spawnInterval = 2.5f;
            currentSpeed = 0.115f;
        } else {
            currentGap = 0.9f;
            spawnInterval = 3.5f;
            currentSpeed = 0.06f;
        }

        velocity += gravity;
        cubeY += velocity;
        floorOffset -= currentSpeed;

        if (floorOffset < -2.5f) floorOffset = 0.0f;

        // Pipe and Boost Spawning
        spawnTimer += currentSpeed;
        if (spawnTimer >= spawnInterval) {
            float maxDiff = 1.0f;
            float diff = ((rand() % 200 - 100) / 100.0f) * maxDiff;
            float newGapY = lastGapY + diff;

            if (newGapY > 1.5f) newGapY = 1.5f;
            if (newGapY < -1.0f) newGapY = -1.0f;

            pipes.push_back(Pipe(8.0f, newGapY));

            if (superModeTimer > 0) {
                activeCoins.emplace_back(8.0f, newGapY - 0.5f);
                activeCoins.emplace_back(8.0f, newGapY);
                activeCoins.emplace_back(8.0f, newGapY + 0.5f);
            } else {
                // 0.5% Chance to spawn a star (super mode)
                bool spawnStar = (rand() % 200) < 1;
                if (spawnStar) {
                    activeStars.emplace_back(8.0f, newGapY);
                } else {
                    // 1% Chance to spawn a heart (extra live)
                    bool spawnHeart = (rand() % 100) < 1;
                    if (spawnHeart) {
                        activeHearts.emplace_back(8.0f, newGapY);
                    }

                    // 20% Chance to spawn a coin
                    if (!spawnHeart && (rand() % 100) < 20) {
                        activeCoins.emplace_back(8.0f, newGapY);
                    }
                }
            }

            lastGapY = newGapY;
            spawnTimer = 0.0f;
        }

        // Check for player collisions
        for (auto it = pipes.begin(); it != pipes.end(); ) {
            bool offScreen = it->update(currentSpeed);

            if (invincibilityTimer <= 0.0f && it->checkCollision(cubeY, cubeSize, currentGap)) {
                if (extraLives > 0) {
                    extraLives--;
                    currentSpeed = 0.06f; 
                    velocity = jumpForce;
                    invincibilityTimer = 2.0f;
                } else {
                    currentState = GAMEOVER;
                }
            }

            if (it->hasBeenPassed(-1.5f)) {
                it->setPassed(true);
                score++;
                currentSpeed += 0.00025f;
            }

            if (offScreen) {
                it = pipes.erase(it);
            } else {
                ++it;
            }
        }

        if (cubeY < -2.8f || cubeY > 3.8f) currentState = GAMEOVER;

        // Collectibles
        updateCollectibles(activeCoins, [&]() {
            score += 10;
        });

        updateCollectibles(activeHearts, [&]() {
            extraLives++;
        });

        updateCollectibles(activeStars, [&]() {
            superModeTimer = 10.0f;
        });

        // Volumetric Cloud Spawning
        updateClouds();

        // Mountain spawning
        updateMountains();
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void initLighting() {
    glEnable(GL_LIGHTING);   // Master switch for lights
    glEnable(GL_LIGHT0);    // Turn on the first light source
    glEnable(GL_COLOR_MATERIAL); // Allows glColor3f to work with lighting
    glEnable(GL_FOG); // Enable Fog

    float fogColor[] = { 0.5f, 0.7f, 1.0f, 1.0f }; // Same as glClearColor
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogf(GL_FOG_DENSITY, 0.02f); // Adjust this to make it more/less misty

    // Set light position (Top-Right-Front)
    float lightPos[] = { 5.0f, 10.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Add a bit of "Ambient" light so shadows aren't pitch black
    float ambientColor[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == ' ' && currentState == PLAYING) velocity = jumpForce;
    if (key == 's' || key == 'S') resetGame();
    if (key == 'm' || key == 'M') currentState = MENU;

    if ((key == 'p' || key == 'P') && (currentState == PLAYING || currentState == PAUSED)) {
        if (currentState == PAUSED) {
            currentState = PLAYING;
        } else {
            currentState = PAUSED;
        }
    }
}

int main(int argc, char** argv) {
    if (!playerModel.loadFromResource(101)) {
        cout << "Failed to load model 101!" << endl;
    }

    if (!heartModel.loadFromResource(102)) {
        cout << "Failed to load model 102!" << endl;
    } else {
        Heart::setModel(&heartModel);
    }

    if (!starModel.loadFromResource(103)) {
        cout << "Failed to load model 103!" << endl;
    } else {
        Star::setModel(&starModel);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-640)/2,
                       (glutGet(GLUT_SCREEN_HEIGHT)-480)/2);

    glutInitWindowSize(1280, 720);
    glutCreateWindow("Flappy Cube");

    // Windows-specific code to set the taskbar and window icon
    HWND hwnd = FindWindow(NULL, "Flappy Cube");
    HICON hIcon = LoadIcon(GetModuleHandle(NULL), "MAINICON");
    if (hIcon) {
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.7, 1.0, 1.0);
    glutDisplayFunc(display);

    glutReshapeFunc([](int w, int h) {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION); glLoadIdentity();
        gluPerspective(45.0, (float)w/h, 1.0, 100.0);
        glMatrixMode(GL_MODELVIEW);
    });

    menuBackground = loadPNGFromResource(201);
    if (menuBackground == 0) {
        cout << "Failed to load texture! Check resource ID 201." << endl;
    }

    initLighting();

    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);

    loadHighScore();

    playerModel.compile();
    heartModel.compile();
    starModel.compile();

    glutMainLoop();

    return 0;
}
