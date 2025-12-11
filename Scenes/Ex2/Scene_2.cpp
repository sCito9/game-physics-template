#include "Scene_2.h"
#include "imgui.h"

void Scene_2::onDraw(Renderer &renderer) {
    renderer.drawCube(
            rb.x_cm, // center position
            rb.r, // rotation
            dimensions, // scale
            glm::vec4(1,0,0,1)); // color

    renderer.drawWireCube(glm::vec3(0),
                          5.0f * dimensions,
                          glm::vec3(1));

    cameraMatrix = renderer.camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);

    hRez = renderer.camera.width;
}

void Scene_2::simulateStep() {
    if (paused || h <= 0.f) return;

    handleInput();

    float realtimeDt = ImGui::GetIO().DeltaTime * speed;
    curDeltaT += realtimeDt;
    for (; curDeltaT >= h; curDeltaT -= h) {
        simulateCuboidRb(&rb, h, F, F_x, nForces);
    }
    nForces = 0;
}

void Scene_2::handleInput() {
    if(ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
        auto drag = ImGui::GetMouseDragDelta(1);
        if(!(drag.x == 0 && drag.y == 0)){
            auto dx = drag.x * right;
            auto dy = -drag.y * up;
            F[nForces] = (dx + dy) / (4.0f * (float)hRez);
            F_x[nForces] = rb.x_cm;
            nForces++;
        }
    }
}


void Scene_2::onGUI() {
    ImGui::LabelText("Info", "[Space] to (un-)pause sim");
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        paused = !paused;
    }
    ImGui::InputFloat("h in s", &h);
    ImGui::InputFloat("speed", &speed);
}

void Scene_2::init() {
    rb = initializeCuboidRb(glm::vec3(0),
                            dimensions,
                            2.0f,
                            glm::vec3(0.f, 0.f, glm::radians(90.0f)),
                            glm::vec3(0),
                            glm::vec3(0));
    printRb(&rb);

    F[0] = glm::vec3(1, 1, 0);
    F_x[0] = glm::vec3(0.3f, 0.5f, 0.25f);
    nForces = 1;

    simulateCuboidRb(&rb, h, F, F_x, nForces);

    F[0] = glm::vec3(0);
    F_x[0] = glm::vec3(0);
    nForces = 0;
}
