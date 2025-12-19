#include "Scenes/Scene.h"
#include "Heat.h"
#include "imgui.h"

class Scene_02 : public Scene {
    void init() override;
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;

    float deltaT = 0.01f;
    float curDeltaT = 0.f;
    int paused = true;
    float speed = 1.f;

    Heat T_t;
    float width;
    float height;
    uint32_t n;
    uint32_t m;

public:
    Scene_02();
};
