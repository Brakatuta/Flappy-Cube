#ifndef MODELOBJ_H
#define MODELOBJ_H

#include <GL/freeglut.h>
#include <vector>
#include <string>

using namespace std;

struct Vec3 { float x, y, z; };
struct Face { int v[3], n[3]; };

class ModelOBJ {
public:
    vector<Vec3> vertices;
    vector<Vec3> normals;
    vector<Face> faces;

    float scaleValue = 1.0f;
    float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;

    GLuint displayList = 0; 

    void setScale(float s) { scaleValue = s; }
    void setRotation(float x, float y, float z) { rotX = x; rotY = y; rotZ = z; }

    ModelOBJ();
    bool load(const char* filename);
    bool loadFromResource(int resourceID);
    bool parseStream(std::istream& stream);
    
    // method to bake the geometry into GPU memory
    void compile(); 
    
    void draw();
};

#endif