#include "image.h"

#include <GL/glext.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL/freeglut.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


GLuint loadPNGFromResource(int resourceID) {
    // 1. Get the raw data from the EXE resources
    HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(resourceID), RT_RCDATA);
    if (!hRes) return 0;

    DWORD size = SizeofResource(NULL, hRes);
    unsigned char* pData = (unsigned char*)LockResource(LoadResource(NULL, hRes));

    // 2. Decode PNG memory to raw pixels
    int w, h, channels;
    unsigned char* img = stbi_load_from_memory(pData, size, &w, &h, &channels, 4);
    if (!img) return 0;

    // 3. Upload to OpenGL
    GLuint tid;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(img);
    return tid;
}
