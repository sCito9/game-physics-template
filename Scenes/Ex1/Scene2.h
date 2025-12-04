#include "Scenes/Scene.h"
#include "Scenes/vec3_util.h"

#include <iostream>
#include <cstring>
#include "thirdparty/glm/glm.hpp"
#include "thirdparty/imgui/imgui.h"

class Scene2 : public Scene
{
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;

    float Δt = 0.005f;
    int Δt_μs = 5000;
    float cur_Δt = 0.f;
    massPoint mp0;
    massPoint mp1;
    spring spr;

    void init() override;
    void calculateEulerStep(massPoint *mp0, massPoint *mp1, spring *spr, float h);
    void printMasspoints(massPoint mp0, massPoint mp1, const char* headlineText);

    public:
        Scene2() = default;
};

