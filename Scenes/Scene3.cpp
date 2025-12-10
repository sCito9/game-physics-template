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
    ImGui::Text("realDeltaTime: actually calls the update function every timestep frames. \n"
                "   -> with a timestep of 2 it will only call it every 2 seconds, which will look laggy. \n"
                "   Disable to simulate a custom passing of time, where the update function gets called every frame\n"
                "Space: Stop and Resume the Simulation\n");
    ImGui::SliderFloat("TimeStep", &timeStep, 0.001f, 2.0f);
    if (ImGui::IsKeyReleased(ImGuiKey_Space)) {
        shouldSimulate = !shouldSimulate;
    }
    ImGui::Checkbox("realDeltaTime", &realDeltaTime);
}

void Scene3::simulateStep() {
    if (!shouldSimulate) {
        return;
    }

    if (realDeltaTime) {
        float realTimeDt = ImGui::GetIO().DeltaTime;

        currentTime += realTimeDt;
        for (; currentTime >= timeStep; currentTime -= timeStep) {
            simStep(timeStep);
        }
    }
    else {
        simStep(timeStep);
    }
}
