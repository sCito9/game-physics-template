#include "Scene.h"
#include "ex1_util.h"

#include <iostream>
#include <cstring>
#include <glm/glm.hpp>


class Scene1 : public Scene
{
    void init() override;

    void printMasspoints(massPoint mp0, massPoint mp1, const char* headlineText);
    void calculateEulerStep(massPoint *mp0, massPoint *mp1, spring *spr, float h);
    void calculateMidpointStep(massPoint *mp0, massPoint *mp1, spring *spr, float h);
};

