#include "Scenes/Scene.h"
#include "Rigidbody.h"
#include "util/CollisionDetection.h"

class Scene_3 : public Scene {
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
    float sensitivity = 1.0f;

    const glm::vec3 dimensions = glm::vec3(1.0f, 1.0f, 2.0f);
    const glm::vec3 rb1_x_0 = glm::vec3(-3, 0, 8);
    const glm::vec3 rb2_x_0 = glm::vec3(3, 0, 8);
    const float Mass = 2.0f;
    const glm::vec3 rb1_r_0 = glm::vec3(0.f, 0.f, glm::radians(90.0f));
    const glm::vec3 rb2_r_0 = glm::vec3(glm::radians(0.0f), glm::radians(45.0f), glm::radians(45.0f));
    CuboidRigidbody rb1;
    CuboidRigidbody rb2;

    glm::vec3 F[3];
    glm::vec3 F_x[3];
    uint8_t nForces;
};
