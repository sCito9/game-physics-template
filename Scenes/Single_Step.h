//
// Created by felix on 19.11.2025.
//

#ifndef SINGLE_STEP_H
#define SINGLE_STEP_H
#include "Scene.h"
#include "Rigidbody.h"


class Single_Step : public Scene
{
    void init();

    float step_size = 2.f;

    //pre compute
    Cube cube;
};


#endif //SINGLE_STEP_H
