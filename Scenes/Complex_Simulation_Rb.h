//
// Created by felix on 29.11.2025.
//

#ifndef COMPLEX_SIMULATION_RB_H
#define COMPLEX_SIMULATION_RB_H
#include "Scene.h"
#include "util/CollisionDetection.h"
#include "Rigidbody.h"


class Complex_Simulation_Rb : public Scene
{
    void init();
    void onDraw(Renderer& renderer) override;
    void simulateStep() override;
    void onGUI() override;


    float step_size = 0.01f;
    float cur_delta_time = 0.f;

    float c = 1.f;
    bool gravity = false;


    //pre compute
    Cube cube_1, cube_2, cube_3, cube_4, cube_5;

    std::vector<Cube*> cubes;

    //External Forces
    glm::vec3 F = glm::vec3(0.f);

    float force_amount[3] = {3.f, 0.f, 2.f};
    float force_position[3] = {0.f, 0.f, 0.f};
};


#endif //COMPLEX_SIMULATION_RB_H
