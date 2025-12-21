#include "Scenes/Scene.h"
#include "Heat.h"
#include "imgui.h"

enum Scheme {
    explic,
    implic
};

class Scene_04 : public Scene {
    void init() override;
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;

    float deltaT = 0.01f;
    float curDeltaT = 0.f;
    bool realTime = true;
    int paused = true;
    float speed = 1.f;
    float inputHeat = 0.3f;

    Scheme scheme;
    void (Heat::* stepFunc) ();

    Heat T_t;
    float width;
    float height;
    uint32_t n;
    uint32_t m;

public:
    Scene_04();
};

