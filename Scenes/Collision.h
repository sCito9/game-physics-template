//
// Created by felix on 28.11.2025.
//

#ifndef COLLISION_H
#define COLLISION_H
#include "Scene.h"
#include "util/CollisionDetection.h"


class Collision : public Scene
{
    void init();

    glm::quat multiplication(glm::quat q1, glm::quat q2);
    void onDraw(Renderer& renderer) override;
    void simulateStep() override;
    void onGUI() override;
    void resetForces();
    void simulateCube(Cube* obj);
    void simulatePointsWorldSpace(Cube* obj);
    glm::mat4 getWorldFromObj(Cube cube);
    void checkCollisions(Cube* cube_array[]);
    void calculateCollision(CollisionInfo hitInfo, Cube* cube_1, Cube* cube_2);

    float step_size = 0.01f;
    float cur_delta_time = 0.f;

    float c = 1.f;

    //pre compute
    Cube cube_1 = {
        glm::vec3(-6.f, 0.f, 0.f), glm::vec3(3.f, 0.f, 0.f),
        //point positions:
        {
            glm::vec3(-2.f, 2.f, 2.f), glm::vec3(-2.f, 2.f, -2.f),
            glm::vec3(-2.f, -2.f, -2.f), glm::vec3(-2.f, -2.f, 2.f),
            glm::vec3(2.f, 2.f, 2.f), glm::vec3(2.f, 2.f, -2.f),
            glm::vec3(2.f, -2.f, -2.f), glm::vec3(2.f, -2.f, 2.f)
        },
        {
            glm::vec3(-8.f, 2.f, 2.f), glm::vec3(-8.f, 2.f, -2.f),
            glm::vec3(-8.f, -2.f, -2.f), glm::vec3(-8.f, -2.f, 2.f),
            glm::vec3(-6.f, 2.f, 2.f), glm::vec3(-6.f, 2.f, -2.f),
            glm::vec3(-6.f, -2.f, -2.f), glm::vec3(-6.f, -2.f, 2.f)
        },
        {0.f, 0.f, 0.f, 0.f}, glm::inverse(
            glm::mat3(
                8.f, 0.f, 0.f,
                0.f, 8.f, 0.f,
                0.f, 0.f, 8.f
            )),
        glm::inverse(glm::mat3(
            8.f, 0.f, 0.f,
            0.f, 8.f, 0.f,
            0.f, 0.f, 8.f
        )),
        glm::vec3(0.f, 0.f, 0.f), 3.f, glm::vec3(4.f), false
    };

    Cube cube_2 = {
        glm::vec3(3.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f),
        //point positions:
        {
            glm::vec3(-2.f, 2.f, 2.f), glm::vec3(-2.f, 2.f, -2.f),
            glm::vec3(-2.f, -2.f, -2.f), glm::vec3(-2.f, -2.f, 2.f),
            glm::vec3(2.f, 2.f, 2.f), glm::vec3(2.f, 2.f, -2.f),
            glm::vec3(2.f, -2.f, -2.f), glm::vec3(2.f, -2.f, 2.f)
        },
        {
            glm::vec3(1.f, 2.f, 2.f), glm::vec3(1.f, 2.f, -2.f),
            glm::vec3(1.f, -2.f, -2.f), glm::vec3(1.f, -2.f, 2.f),
            glm::vec3(5.f, 2.f, 2.f), glm::vec3(5.f, 2.f, -2.f),
            glm::vec3(5.f, -2.f, -2.f), glm::vec3(5.f, -2.f, 2.f)
        },
        {0.f, 0.f, 45.f, 45.f}, glm::inverse(
            glm::mat3(
                64.f / 12.f, 0.f, 0.f,
                0.f, 64.f / 12.f, 0.f,
                0.f, 0.f, 64.f / 12.f
            )),
        glm::inverse(glm::mat3(
            64.f / 12.f, 0.f, 0.f,
            0.f, 64.f / 12.f, 0.f,
            0.f, 0.f, 64.f / 12.f
        )),
        glm::vec3(0.f, 0.f, 0.f), 2.f, glm::vec3(4.f), false
    };
    Cube* cubes[2] = {&cube_1, &cube_2};
    //initialize w
    glm::vec3 w = glm::vec3(0.f);


    //External Forces
    glm::vec3 F = glm::vec3(0.f);
    glm::vec3 q = glm::vec3(0.f);
};


#endif //COLLISION_H
