//
// Created by felix on 28.11.2025.
//

#ifndef COLLISION_H
#define COLLISION_H
#include "Scene.h"
#include "util/CollisionDetection.h"
#include "Rigidbody.h"


class Collision : public Scene
{
    void init();
    void onDraw(Renderer& renderer) override;
    void simulateStep() override;
    void onGUI() override;


    float step_size = 0.01f;
    float cur_delta_time = 0.f;

    float c = 1.f;

    //pre compute
    Cube cube_1, cube_2;
    std::vector<Cube*> cubes;

    //External Forces
    glm::vec3 F = glm::vec3(0.f);
};


#endif //COLLISION_H
