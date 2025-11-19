//
// Created by felix on 19.11.2025.
//

#ifndef SINGLE_STEP_H
#define SINGLE_STEP_H
#include "Scene.h"


class Single_Step : public Scene
{
    void init();
    float step_size = 2.f;

    //pre compute
    float M = 2.f;
    glm::vec3 x_cm = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 v_cm = glm::vec3(0.f, 0.f, 0.f);

    //extends:
    float a = 1.f;
    float b = 0.6f;
    float c = 0.5f;

    //initialize some values:
    glm::vec3 r = glm::vec3(0.f, 0.f, glm::pi / 2.f);

    glm::mat3 I = glm::mat3(1.f);

    glm::vec3 w = glm::vec3(0.f, 0.f, 0.f);


    //forces
    glm::vec3 q = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 F = glm::vec3(0.f, 0.f, 0.f);
};


#endif //SINGLE_STEP_H
