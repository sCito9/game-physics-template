//
// Created by tonib on 29.11.2025.
//

#include "Scene3.h"

#include "imgui.h"

void Scene3::init() {
    glm::mat3 initialRotation = glm::mat3(cos(glm::pi<float>() / 4), -sin(glm::pi<float>() / 4), 0,
                                        sin(glm::pi<float>() / 4), cos(glm::pi<float>() /4), 0,
                                        0, 0, 1);
    glm::mat3 initalRotation2 = glm::mat3(1, 0, 0,
                                        cos(glm::pi<float>() / 4), -sin(glm::pi<float>() / 4), 0,
                                        sin(glm::pi<float>() / 4), cos(glm::pi<float>() /4), 0);
    cubes.emplace_back(new Rigidbody_Cube({0, 0, 0}, glm::toQuat(initialRotation * initalRotation2), {0, 0, 0}, 1.f, 1.f, 1.f, 2));
    cubes.emplace_back(new Rigidbody_Cube({2, 0, 0}, {0, 0, 0, 0}, {-.1f, 0, 0}, 1.f, 1.f, 1.f, 2));

}

void Scene3::onGUI() {
    ImGui::SliderFloat("TimeStep", &timeStep, 0.001f, 2.0f);
    if (ImGui::IsKeyReleased(ImGuiKey_Space)) {
        shouldSimulate = !shouldSimulate;
    }
}

void Scene3::simulateStep() {
    if (!shouldSimulate) {
        return;
    }
    float realTimeDt = ImGui::GetIO().DeltaTime;

    currentTime += realTimeDt;
    for (; currentTime >= timeStep; currentTime -= timeStep) {
        simStep(timeStep);
    }
}
