#include "Scenes/Scene.h"
#include "Heat.h"

class Scene_01 : public Scene {
    void init() override;
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;

    Heat T_t;
    float width;
    float height;
public:
    Scene_01();
};
