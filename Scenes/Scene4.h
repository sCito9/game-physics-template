#include "Scene.h"
#include "ex1_util.h"

#include <iostream>
#include <cstring>
#include <glm/glm.hpp>
#include <imgui.h>

struct mSpring {
    glm::vec3 F01;
    float restLen;
    float stiffness;
    int mp0_index;
    int mp1_index;
};

struct mMassPoint {
    glm::vec3 F;
    glm::vec3 x;
    glm::vec3 v;
    float mass;
};

enum Method {
    Euler ,
    Midpoint
};

class Scene4 : public Scene
{
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;
    void init() override;

    float deltaT = 0.005f;
    float curDeltaT = 0.f;
    int paused = true;

    static const int nMasspoints = 10;
    mMassPoint massPoints[nMasspoints];
    mMassPoint massPointsMid[nMasspoints];
    static const int nSprings = 37;
    mSpring springs[nSprings];
    float mass = 10.f;

    glm::vec3 extF = glm::vec3(0);
    const glm::vec3 g = glm::vec3(0, 0, -9.81f);
    float global_stiffness_multiplier = 1.f;

    const float floor = -4.2f;
    const float ceiling = 13.f;

    void (Scene4::* simFunc) ();
    Method selectedMethod;
    void simulateSceneEuler();
    void simulateSceneMidpoint();

    void computeElasticSpringForce(mSpring* spr, mMassPoint* mps);
    void updateSpring(mSpring* spr);

    void calculateEulerStep(mMassPoint* mp, float h);
    void calculateMidpointStep(mMassPoint* mp, glm::vec3 midF);

    public:
        Scene4() = default;
};

