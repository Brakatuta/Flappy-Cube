#ifndef HEART_H
#define HEART_H

#include "Boost.h"
#include "../utils/ModelOBJ.h"

class Heart : public Boost {
public:
    Heart(float startX, float startY);
    void draw() const override;
    static void setModel(ModelOBJ* model);

private:
    static ModelOBJ* heartModel;
};

#endif