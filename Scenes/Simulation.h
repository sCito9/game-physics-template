//
// Created by felix on 27.11.2025.
//

#ifndef SIMULATION_H
#define SIMULATION_H
#include "Scene.h"


class Simulation : public Scene
{
    void init();

    glm::quat multiplication(glm::quat q1, glm::quat q2);
    void onDraw(Renderer& renderer) override;
    void simulateStep() override;
    void onGUI() override;
    void resetForces();
    void simulateCube(Cube* obj);
    void simulatePointsWorldSpace(Cube* obj);

    float step_size = 0.01f;
    float cur_delta_time = 0.f;

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

    //initialize w
    glm::vec3 w = glm::vec3(0.f);


    //External Forces
    glm::vec3 F = glm::vec3(0.f);
    glm::vec3 q = glm::vec3(0.f);


    float force_amount[3] = {3.f, 0.f, 2.f};
    float force_position[3] = {0.f, 0.f, 0.f};
};


#endif //SIMULATION_H
