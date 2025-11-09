//
// Created by tonib on 07.11.2025.
//
#include "Scene.h"
#include "Spring.h"

class Scene_1 : public Scene
{
    void init() override;

    void calcEulerStep(float timeStep, Spring *s);

    void calcMidpointStep(float timeStep, Spring *s);
};
