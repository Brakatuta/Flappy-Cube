#include "ModelOBJ.h"
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring> // For memcpy

// Ensure the header is packed exactly as the Python script writes it
#pragma pack(push, 1)
struct BinHeader {
    unsigned short vCount;
    unsigned short nCount;
    unsigned short fCount;
};
#pragma pack(pop)

ModelOBJ::ModelOBJ() : 
    vertices(nullptr), normals(nullptr), faces(nullptr),
    numVertices(0), numNormals(0), numFaces(0),
    rotX(0), rotY(0), rotZ(0), scaleValue(1.0f), displayList(0) {}

ModelOBJ::~ModelOBJ() {
    clear();
    if (displayList > 0) glDeleteLists(displayList, 1);
}

void ModelOBJ::clear() {
    if (vertices) free(vertices);
    if (normals) free(normals);
    if (faces) free(faces);
    vertices = nullptr;
    normals = nullptr;
    faces = nullptr;
    // Do not reset num counts here if you need them for logic later, 
    // but usually, clear() means a full reset.
}

bool ModelOBJ::loadFromResource(int resourceID) {
    HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(resourceID), "OBJFILE");
    if (!hRes) return false;

    HGLOBAL hData = LoadResource(NULL, hRes);
    if (!hData) return false;

    const unsigned char* ptr = (const unsigned char*)LockResource(hData);
    
    clear(); 

    BinHeader header;
    memcpy(&header, ptr, sizeof(BinHeader));
    ptr += sizeof(BinHeader);

    numVertices = header.vCount;
    numNormals = header.nCount;
    numFaces = header.fCount;

    // Allocate memory
    vertices = (Vec3*)malloc(numVertices * sizeof(Vec3));
    normals = (Vec3*)malloc(numNormals * sizeof(Vec3));
    faces = (Face*)malloc(numFaces * sizeof(Face));

    // Copy Vertices
    memcpy(vertices, ptr, numVertices * sizeof(Vec3));
    ptr += numVertices * sizeof(Vec3);

    // Copy Normals
    memcpy(normals, ptr, numNormals * sizeof(Vec3));
    ptr += numNormals * sizeof(Vec3);

    // Convert 16-bit indices to 32-bit int
    for (int i = 0; i < numFaces; i++) {
        const unsigned short* f16 = (const unsigned short*)ptr;
        faces[i].v[0] = f16[0];
        faces[i].v[1] = f16[1];
        faces[i].v[2] = f16[2];
        faces[i].n[0] = f16[3];
        faces[i].n[1] = f16[4];
        faces[i].n[2] = f16[5];
        ptr += 12; 
    }

    return true;
}

void ModelOBJ::compile() {
    if (numVertices == 0 || !vertices) return;

    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);
    
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < numFaces; i++) {
        for (int j = 0; j < 3; j++) {
            // Apply normals if they exist
            if (numNormals > 0 && normals) {
                int nIdx = faces[i].n[j];
                glNormal3f(normals[nIdx].x, normals[nIdx].y, normals[nIdx].z);
            }
            int vIdx = faces[i].v[j];
            glVertex3f(vertices[vIdx].x, vertices[vIdx].y, vertices[vIdx].z);
        }
    }
    glEnd();
    glEndList();
    
    // RAM is saved by deleting the raw data after it's sent to the GPU (Display List)
    clear();
}

void ModelOBJ::draw() {
    if (displayList == 0) return;

    glPushMatrix();
        // Apply transformations that were "missing"
        glRotatef(rotX, 1, 0, 0);
        glRotatef(rotY, 0, 1, 0);
        glRotatef(rotZ, 0, 0, 1);
        glScalef(scaleValue, scaleValue, scaleValue);

        glCallList(displayList);
    glPopMatrix();
}