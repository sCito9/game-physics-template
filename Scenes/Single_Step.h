//
// Created by felix on 19.11.2025.
//

#ifndef SINGLE_STEP_H
#define SINGLE_STEP_H
#include "Scene.h"


class Single_Step : public Scene
{
    void init();

    glm::quat multiplication(glm::quat q1, glm::quat q2);


    float step_size = 2.f;

    //pre compute
    Cube cube = {
        glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f),
        //point positions:
        {
            glm::vec3(-1.f, -0.6f, -0.5f), glm::vec3(1.f, -0.6f, -0.5f),
            glm::vec3(1.f, 0.6f, -0.5f), glm::vec3(1.f, 0.6f, 0.5f),
            glm::vec3(-1.f, 0.6f, -0.5f), glm::vec3(-1.f, 0.6f, 0.5f),
            glm::vec3(1.f, -0.6f, 0.5f), glm::vec3(-1.f, -0.6f, 0.5f)
        },
        {
            glm::vec3(-1.f, -0.6f, -0.5f), glm::vec3(1.f, -0.6f, -0.5f),
            glm::vec3(1.f, 0.6f, -0.5f), glm::vec3(1.f, 0.6f, 0.5f),
            glm::vec3(-1.f, 0.6f, -0.5f), glm::vec3(-1.f, 0.6f, 0.5f),
            glm::vec3(1.f, -0.6f, 0.5f), glm::vec3(-1.f, -0.6f, 0.5f)
        },
        {0.f, 0.f, 0.f, 90.f}, glm::inverse(
            glm::mat3(
                4.88f / 12.f, 0.f, 0.f,
                0.f, 10.88f / 12.f, 0.f,
                0.f, 0.f, 10.f / 12.f
            )),
        glm::inverse(glm::mat3(
            4.88f / 12.f, 0.f, 0.f,
            0.f, 10.88f / 12.f, 0.f,
            0.f, 0.f, 10.f / 12.f
        )),
        glm::vec3(0.f, 0.f, 0.f), 2.f, glm::vec3(1.f), false
    };
};


#endif //SINGLE_STEP_H
