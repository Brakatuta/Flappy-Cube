#ifndef MODELOBJ_H
#define MODELOBJ_H

#include <GL/freeglut.h>

// Simple Vec3 and Face structures
struct Vec3 { float x, y, z; };
struct Face { int v[3], n[3]; };

class ModelOBJ {
public:
    // Replaced std::vector with raw pointers
    Vec3* vertices;
    Vec3* normals;
    Face* faces;

    // Keep track of counts manually
    int numVertices;
    int numNormals;
    int numFaces;

    float scaleValue;
    float rotX, rotY, rotZ;
    GLuint displayList;

    ModelOBJ();
    ~ModelOBJ(); // Destructor to free memory

    void setScale(float s) { scaleValue = s; }
    void setRotation(float x, float y, float z) { rotX = x; rotY = y; rotZ = z; }

    bool loadFromResource(int resourceID);
    void compile(); 
    void draw();

private:
    void clear(); // Helper to free existing data
};

#endif