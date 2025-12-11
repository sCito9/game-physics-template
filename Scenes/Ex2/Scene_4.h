#include "Scenes/Scene.h"
#include "Rigidbody.h"
#include "util/CollisionDetection.h"

class Scene_4 : public Scene {
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

    static const int nRigidbodies = 3;
    const glm::vec3 dimensions[nRigidbodies] = {
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            //glm::vec3(1.0f, 1.0f, 1.0f),
            //glm::vec3(1.0f, 1.0f, 1.0f),
    };
    const glm::vec3 rb_x_0[nRigidbodies] = {
            glm::vec3(-5, 0, 0),
            glm::vec3(0, 0, 0),
            glm::vec3(5, 0, 0),
            //glm::vec3(0, 0, 20),
            //glm::vec3(0, 0, 10),
    };
    const float Mass[nRigidbodies] = {
            2.0f,
            2.0f,
            2.0f,
            //2.0f,
            //2.0f,
    };
    const glm::vec3 rb_r_0[nRigidbodies] = {
            glm::vec3(0.f, 0.f, 0.0f),
            glm::vec3(0.f, 0.f, 0.0f),
            glm::vec3(0.f, 0.f, 0.0f),
            //glm::vec3(0.f, 0.f, 0.0f),
            //glm::vec3(0.f, 0.f, 0.0f),
    };
    CuboidRigidbody rb[nRigidbodies];

    glm::vec3 F;

    CuboidRigidbody floor;
    const float floorLocation = -25;
    const glm::vec3 floorDimensions = glm::vec3(-2 * floorLocation, -2 * floorLocation, .1f);
    glm::mat4 floorMatrix;
};
