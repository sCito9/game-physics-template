//
// Created by tonib on 30.11.2025.
//

#include "Scene4.h"

#include "imgui.h"

void Scene4::init() {
    cubes.emplace_back(new Rigidbody_Cube({1, .3, .71}, {12, 35, 1, 0}, {0, 0, 0}, 2.f, 0.6f, 1.34f, 1.5f));
    cubes.emplace_back(new Rigidbody_Cube({2, -1.5, -0.2}, {2, 8, .38, 2.3}, {-.1f, 0, 0}, 1.2f, 0.34f, 2.5453f, 3.f));
    cubes.emplace_back(new Rigidbody_Cube({0.1, 1, -1.5}, {123, 3.5, 45.8, 54}, {0, 0, 0}, 0.34f, 0.6f, 0.45f, .6f));
    cubes.emplace_back(new Rigidbody_Cube({-2, 1.8, -0.3}, {0.21, 0.3, 0.26, 0.887}, {-.1f, 0, 0}, 2.675f, 2.67f, 1.34f, 5.f));
    cubes.emplace_back(new Rigidbody_Cube({0, 0, -3.f}, {0, 0, 0, 0}, {0, 0, 0}, 100.f, 100.f, 1.f, 9999999999.f));
}

void Scene4::onGUI() {
    ImGui::SliderFloat("TimeStep", &timeStep, 0.001f, 2.0f);
    ImGui::SliderFloat("CollisionElasticity", &collisionElasticity, 0.f, 1.f);
    if (ImGui::IsKeyReleased(ImGuiKey_Space)) {
        shouldSimulate = !shouldSimulate;
    }
}

void Scene4::simulateStep() {
    if (!shouldSimulate) {
        return;
    }

    for (auto& cube : cubes) {
        addForce(cube, glm::vec3(0, 0, 0), glm::vec3(0, 0, -9.81f));
    }

    float realTimeDt = ImGui::GetIO().DeltaTime;

    currentTime += realTimeDt;
    for (; currentTime >= timeStep; currentTime -= timeStep) {
        simStep(timeStep);
    }
}
