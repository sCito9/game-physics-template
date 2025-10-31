//
// Created by felix on 31.10.2025.
//

#ifndef SINGLE_STEP_H
#define SINGLE_STEP_H
#include "Scene.h"


class Single_Step : public Scene
{
    void init();
    Point point_0 = {glm::vec3(0.f, 0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f), 10.f, glm::vec4(1.f, 0.f, 0.f, 1.f), false};
    Point point_1 = {glm::vec3(0.f, 2.f, 0.f), glm::vec3(1.f, 0.f, 0.f), 10.f, glm::vec4(0.f, 0.f, 1.f, 1.f), false};

    Point points[2] = {point_0, point_1};

    Spring spring = {0, 1, 40.f, 1.f};
};


#endif //SINGLE_STEP_H
