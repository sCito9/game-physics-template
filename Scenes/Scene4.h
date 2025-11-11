#include "Scene.h"
#include "ex1_util.h"

#include <iostream>
#include <cstring>
#include <glm/glm.hpp>
#include <imgui.h>

class Scene4 : public Scene
{
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;
    void init() override;

    float Δt = 0.005f;
    int Δt_μs = 5000;
    massPoint* massPoints;
    int nMasspoints = 10;
    spring* springs;
    int nSprings = 10;
    float cur_Δt = 0.f;
    void (Scene4::* stepFunc) (massPoint*, massPoint*, spring*, float);

    void simulateAllMasspoints(float h);
    void calculateEulerStep(massPoint *mp0, massPoint *mp1, spring *spr, float h);
    void calculateMidpointStep(massPoint *mp0, massPoint *mp1, spring *spr, float h);
    void printMasspoints(massPoint* masspoints, int nMasspoints, const char* headlineText);

    public:
        Scene4() {}
};

