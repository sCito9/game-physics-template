#include "Scenes/Scene.h"
#include "Rigidbody.h"

class Scene_1 : public Scene {
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;
    void init() override;

    CuboidRigidbody rb;

    glm::vec3 F;
    glm::vec3 F_x;

    float h = 2.0f;
};
