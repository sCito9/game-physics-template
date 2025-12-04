#include "Scenes/Scene.h"
#include "Scenes/vec3_util.h"

#include <iostream>
#include <cstring>
#include "thirdparty/glm/glm.hpp"
#include "thirdparty/imgui/imgui.h"

class Scene3 : public Scene
{
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;
    void init() override;

    float Δt = 0.005f;
    int Δt_μs = 5000;
    massPoint mp0;
    massPoint mp1;
    spring spr;
    float cur_Δt = 0.f;

    void calculateEulerStep(massPoint *mp0, massPoint *mp1, spring *spr, float h);
    void calculateMidpointStep(massPoint *mp0, massPoint *mp1, spring *spr, float h);
    void printMasspoints(massPoint mp0, massPoint mp1, const char* headlineText);

    public:
        Scene3() {}
};

