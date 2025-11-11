//
// Created by felix on 31.10.2025.
//

#include "Scene.h"


class Euler_Simulation : public Scene
{
    virtual void onDraw(Renderer& renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;
    Point point_0 = {glm::vec3(0.f, 0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f), 10.f, glm::vec4(1.f, 0.f, 0.f, 1.f), false};
    Point point_1 = {glm::vec3(0.f, 2.f, 0.f), glm::vec3(1.f, 0.f, 0.f), 10.f, glm::vec4(0.f, 0.f, 1.f, 1.f), false};

    Point points[2] = {point_0, point_1};

    Spring spring = {0, 1, 40.f, 1.f};

    float time_step = 0.005f;
    float cur_delta_time = 0.f;
};


