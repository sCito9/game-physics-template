#include "Scene_1.h"

void Scene_1::init() {
    rb = initializeCuboidRb(glm::vec3(0),
                            glm::vec3(1.0f, 0.6f, 0.5f),
                            2.0f,
                            glm::vec3(0.f, 0.f, glm::radians(90.0f)),
                            glm::vec3(0),
                            glm::vec3(0));
    //printRb(&rb);

    F = glm::vec3(1, 0, 0);
    F_x = glm::vec3(0.3f, 0.5f, 0.25f);
    simulateCuboidRb(&rb, h, &F, &F_x, 1);

    std::cout << "Result: " << std::endl;
    printRb(&rb);

    auto p = glm::vec3(-0.3f, -0.5f, -0.25f);
    std::cout << "world space velocity of point " << p << " : " << rb.v_cm + cross(rb.w, p) << std::endl;
}

void Scene_1::onDraw(Renderer &renderer) {
    Scene::onDraw(renderer);
}

void Scene_1::onGUI() {
    Scene::onGUI();
}

void Scene_1::simulateStep() {
    Scene::simulateStep();
}
