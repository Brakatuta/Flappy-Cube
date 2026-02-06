#ifndef STAR_H
#define STAR_H

#include "Boost.h"
#include "../utils/ModelOBJ.h"

class Star : public Boost {
public:
    Star(float startX, float startY);
    void draw() const override;
    static void setModel(ModelOBJ* model);

private:
    static ModelOBJ* starModel;
};

#endif