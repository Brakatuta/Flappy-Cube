#include "ModelOBJ.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>

using namespace std;

ModelOBJ::ModelOBJ() : rotX(0), rotY(0), rotZ(0), scaleValue(1.0f) {}

// Core Parser: Handles the actual OBJ logic from any stream
bool ModelOBJ::parseStream(istream& stream) {
    vertices.clear();
    normals.clear();
    faces.clear();

    string line;
    while (getline(stream, line)) {
        stringstream ss(line);
        string prefix;
        ss >> prefix;

        if (prefix == "v") {
            Vec3 v; ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (prefix == "vn") {
            Vec3 n; ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        } else if (prefix == "f") {
            Face f;
            string vertexData;
            for (int i = 0; i < 3; i++) {
                if (!(ss >> vertexData)) break;
                size_t firstSlash = vertexData.find('/');
                size_t lastSlash = vertexData.find_last_of('/');

                f.v[i] = stoi(vertexData.substr(0, firstSlash)) - 1;
                if (lastSlash != string::npos && lastSlash != firstSlash) {
                    f.n[i] = stoi(vertexData.substr(lastSlash + 1)) - 1;
                }
            }
            faces.push_back(f);
        }
    }
    return !vertices.empty();
}

// file loader, uses parseStream
bool ModelOBJ::load(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        return false;
    }
    return parseStream(file);
}

// Resource loader for baking into EXE
bool ModelOBJ::loadFromResource(int resourceID) {
    HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(resourceID), "OBJFILE");
    if (!hRes) return false;

    HGLOBAL hData = LoadResource(NULL, hRes);
    if (!hData) return false;

    const char* pData = (const char*)LockResource(hData);
    DWORD dwSize = SizeofResource(NULL, hRes);

    // Convert memory buffer to stream
    string content(pData, dwSize);
    stringstream ss(content);

    return parseStream(ss);
}

void ModelOBJ::draw() {
    glPushMatrix();

    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f);
    glScalef(scaleValue, scaleValue, scaleValue);

    glBegin(GL_TRIANGLES);
    for (auto& f : faces) {
        for (int i = 0; i < 3; i++) {
            if (!normals.empty() && f.n[i] < normals.size()) {
                glNormal3f(normals[f.n[i]].x, normals[f.n[i]].y, normals[f.n[i]].z);
            }
            if (f.v[i] < vertices.size()) {
                glVertex3f(vertices[f.v[i]].x, vertices[f.v[i]].y, vertices[f.v[i]].z);
            }
        }
    }
    glEnd();

    glPopMatrix();
}
