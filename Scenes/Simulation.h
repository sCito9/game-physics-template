//
// Created by felix on 27.11.2025.
//

#ifndef SIMULATION_H
#define SIMULATION_H
#include "Scene.h"
#include "Rigidbody.h"


class Simulation : public Scene
{
    void init();
    void onDraw(Renderer& renderer) override;
    void simulateStep() override;
    void onGUI() override;

    float step_size = 0.01f;
    float cur_delta_time = 0.f;

    //pre compute
    Cube cube;

    //External Forces
    glm::vec3 F = glm::vec3(3.f, 0.f, 2.f);


    float force_amount[3] = {0.f, 0.f, 0.f};
    float force_position[3] = {0.3f, 0.5f, 0.25f};
};


#endif //SIMULATION_H
