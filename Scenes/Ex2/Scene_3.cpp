#include "Scene_3.h"
#include "imgui.h"

void Scene_3::onDraw(Renderer &renderer) {
    renderer.drawCube(
            rb1.x_cm, // center position
            rb1.r, // rotation
            dimensions, // scale
            glm::vec4(1,0,0,1)); // color

    renderer.drawCube(
            rb2.x_cm, // center position
            rb2.r, // rotation
            dimensions, // scale
            glm::vec4(1,0.13f,0.3f,1)); // color

    renderer.drawWireCube(glm::vec3(0),
                          13.f * dimensions,
                          glm::vec3(1));

    cameraMatrix = renderer.camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);

    hRez = renderer.camera.width;
}

void Scene_3::simulateStep() {
    if (paused || h <= 0.f) return;

    handleInput();

    float realtimeDt = ImGui::GetIO().DeltaTime * speed;
    curDeltaT += realtimeDt;
    for (; curDeltaT >= h; curDeltaT -= h) {
        simulateCuboidRb(&rb1, h, F, nForces);
        simulateCuboidRb(&rb2, h, F, nForces);
    }
    nForces = 1;

    glm::mat4 rb1_mat = getWorldFromObj(&rb1, dimensions);
    glm::mat4 rb2_mat = getWorldFromObj(&rb2, dimensions);
    CollisionInfo collisionInfo = collisionTools::checkCollisionSAT(rb1_mat, rb2_mat);

    if (!collisionInfo.isColliding) return;

    glm::vec3 v1 = rb1.v_cm + cross(rb1.w, collisionInfo.collisionPointWorld - rb1.x_cm);
    glm::vec3 v2 = rb2.v_cm + cross(rb2.w, collisionInfo.collisionPointWorld - rb2.x_cm);
    float J = calculateImpulseFromCollision(v1-v2, collisionInfo.normalWorld, &rb1, &rb2);
}

void Scene_3::handleInput() {
    if(ImGui::IsMouseReleased(ImGuiMouseButton_Right)){
        auto drag = ImGui::GetMouseDragDelta(1);
        if(!(drag.x == 0 && drag.y == 0)){
            auto dx = drag.x * right;
            auto dy = -drag.y * up;
            F[nForces] = sensitivity * (dx + dy) / (4.0f * (float)hRez);
            F_x[nForces] = rb1.x_cm;
            nForces++;
        }
    }
}


void Scene_3::onGUI() {
    ImGui::LabelText("Info", "[Space] to (un-)pause sim");
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        paused = !paused;
    }
    ImGui::InputFloat("h in s", &h);
    ImGui::InputFloat("speed", &speed);
    ImGui::InputFloat("sensitivity", &sensitivity);
}

void Scene_3::init() {
    rb1 = initializeCuboidRb(glm::vec3(-3, 0, 8),
                            dimensions,
                            2.0f,
                            glm::vec3(0.f, 0.f, glm::radians(90.0f)),
                            glm::vec3(0),
                            glm::vec3(0));

    rb2 = initializeCuboidRb(glm::vec3(3, 0 ,8),
                            dimensions,
                            2.0f,
                            glm::vec3(glm::radians(0.0f), glm::radians(45.0f), glm::radians(45.0f)),
                            glm::vec3(0),
                            glm::vec3(0));

    F[0] = glm::vec3(0, 0, -9.81f * rb1.M);
    F_x[0] = rb1.x_cm;
    nForces = 1;
    simulateCuboidRb(&rb1, h, F, F_x, nForces);

    F[0] = glm::vec3(0, 0, -9.81f * rb2.M);
    F_x[0] = rb2.x_cm;
    F[1] = glm::vec3(-6942.0f, 0, 0);
    F_x[1] = rb2.x_cm;
    nForces = 2;
    simulateCuboidRb(&rb2, h, F, F_x, nForces);

    nForces = 1;
}
