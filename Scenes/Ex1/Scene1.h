#include "Scenes/Scene.h"
#include "Scenes/vec3_util.h"

#include <iostream>
#include <cstring>
#include "thirdparty/glm/glm.hpp"


class Scene1 : public Scene
{
    void init() override;

    void printMasspoints(massPoint mp0, massPoint mp1, const char* headlineText);
    void calculateEulerStep(massPoint *mp0, massPoint *mp1, spring *spr, float h);
    void calculateMidpointStep(massPoint *mp0, massPoint *mp1, spring *spr, float h);
};

