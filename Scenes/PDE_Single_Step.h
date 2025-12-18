//
// Created by felix on 10.12.2025.
//

#ifndef PDE_SINGLE_STEP_H
#define PDE_SINGLE_STEP_H
#include "Scene.h"


class PDE_Single_Step : public Scene
{
    const float v = 0.1f;
    const float delta_t = 0.1f;
    const float delta_x = 4.f / (8 - 1);
    const float delta_y = 2.f / (5 - 1);
    std::vector<std::vector<float>> M = {
        {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f},
        {0.f, 6.f, 5.f, 1.f, -1.f, -2.f, -1.f, 0.f},
        {0.f, 4.f, 3.f, 0.f, -1.f, -3.f, -1.f, 0.f},
        {0.f, 3.f, 2.f, -1.f, -2.f, -4.f, -2.f, 0.f},
        {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f}
    };


    //Methods
    float solveExplicitHeatInteriorAt(int i, int j, std::vector<std::vector<float>> M_old);
    void solveExplicitMatrix(std::vector<std::vector<float>> M_old);
    void init() override;
    void onDraw(Renderer& renderer) override;
};


#endif //PDE_SINGLE_STEP_H
