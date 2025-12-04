#include "Scenes/Scene.h"
#include "Rigidbody.h"

class Scene_2 : public Scene {
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;
    void init() override;

    float h = 0.001f;
    float speed = 1.0f;
    float curDeltaT = 0.f;
    int paused = true;

    void handleInput();
    int hRez;

    CuboidRigidbody rb;
    const glm::vec3 dimensions = glm::vec3(1.0f, 0.6f, 0.5f);

    glm::vec3 F[3];
    glm::vec3 F_x[3];
    uint8_t nForces;
};
