#ifndef CLOUD_H
#define CLOUD_H

#pragma once

class Cloud {
public:
    Cloud(float x, float y, float z, float w, float h, float d, float speedMulti);

    bool update(float currentSpeed);
    void draw() const;

    float getZ() const { return z; }

private:
    float x, y, z;
    float w, h, d;
    float speedMult;
};

#endif