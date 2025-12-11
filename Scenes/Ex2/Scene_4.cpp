#include "Scene_4.h"
#include "imgui.h"

void Scene_4::onDraw(Renderer &renderer) {
    for (int i = 0; i < nRigidbodies; ++i) {
        renderer.drawCube(
                rb[i].x_cm, // center position
                rb[i].r, // rotation
                dimensions[i], // scale
                glm::vec4(1, ((float)i)/nRigidbodies, 1 - ((float)i)/nRigidbodies, 1)); // color
    }

    renderer.drawWireCube(glm::vec3(0),
                          glm::vec3(-2 * floorLocation),
                          glm::vec3(1));
    /*
    renderer.drawCube(
            floor.x_cm, // center position
            floor.r, // rotation
            floorDimensions, // scale
            glm::vec4(.68f, .85f, .9f, 1)); // color
    */

    cameraMatrix = renderer.camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);

    hRez = renderer.camera.width;
}

void Scene_4::simulateStep() {
    if (paused || h <= 0.f) return;

    handleInput();

    float realtimeDt = ImGui::GetIO().DeltaTime * speed;
    curDeltaT += realtimeDt;
    for (; curDeltaT >= h; curDeltaT -= h) {
        for (auto & i : rb) {
            i.f[1] = F;
            simulateCuboidRb(&i, h);
            i.f[1] = glm::vec3(0);
        }
    }

    glm::mat4 rb_mat[nRigidbodies];
    for (int i = 0; i < nRigidbodies; ++i) {
        rb_mat[i] = getWorldFromObj(&rb[i], dimensions[i]);
    }

    for (int i = 0; i < nRigidbodies; ++i) {
        for (int j = i + 1; j < nRigidbodies; ++j) {

            CollisionInfo collisionInfo = collisionTools::checkCollisionSAT(rb_mat[i], rb_mat[j]);
            if (collisionInfo.isColliding) {
                glm::vec3 v1 = rb[i].v_cm + cross(rb[i].w, collisionInfo.collisionPointWorld - rb[i].x_cm);
                glm::vec3 v2 = rb[j].v_cm + cross(rb[j].w, collisionInfo.collisionPointWorld - rb[j].x_cm);
                float J = calculateImpulseFromCollision(v1-v2, collisionInfo.normalWorld, &rb[i], &rb[j]);
                applyImpulse(J, collisionInfo.normalWorld, &rb[i], &rb[j]);
            }
        }
    }


    /* floor collisions
    for (int i = 0; i < nRigidbodies; ++i) {
        CollisionInfo collisionInfo = collisionTools::checkCollisionSAT(rb_mat[i], floorMatrix);
        if (collisionInfo.isColliding) {
            glm::vec3 v = rb[i].v_cm + cross(rb[i].w, collisionInfo.collisionPointWorld - rb[i].x_cm);
            float J = calculateImpulseFromCollision(v, collisionInfo.normalWorld, &rb[i], &floor);
            std::cout << collisionInfo.normalWorld << std::endl;
            applyImpulse(J, collisionInfo.normalWorld, &rb[i]);
        }
    }*/
}

void Scene_4::handleInput() {
    if(ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
        auto drag = ImGui::GetMouseDragDelta(1);
        if(!(drag.x == 0 && drag.y == 0)){
            auto dx = drag.x * right;
            auto dy = -drag.y * up;
            F = sensitivity * (dx + dy) / (4.0f * (float)hRez);
        }
    }
}


void Scene_4::onGUI() {
    ImGui::LabelText("Info", "[Space] to (un-)pause sim");
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        paused = !paused;
    }
    ImGui::InputFloat("h in s", &h);
    ImGui::InputFloat("speed", &speed);
    ImGui::InputFloat("sensitivity", &sensitivity);
}

void Scene_4::init() {
    for (int i = 0; i < nRigidbodies; ++i) {
        rb[i] = initializeCuboidRb(rb_x_0[i],
                                 dimensions[i],
                                 Mass[i],
                                 rb_r_0[i],
                                 glm::vec3(0),
                                 glm::vec3(0));
        rb[i].f[1] = normalize(-rb[i].x_cm) * 13000.0f;
    }

    floor = initializeHeavyCuboidRb(glm::vec3(0, 0, floorLocation),
                                    floorDimensions,
                                    glm::vec3(0),
                                    glm::vec3(0),
                                    glm::vec3(0));
    floorMatrix = getWorldFromObj(&floor, floorDimensions);


    rb[1].f[1] = glm::vec3(-6942.0f, 0, 0);
    for (auto & i : rb) {
        simulateCuboidRb(&i, h);
        i.f[1] = glm::vec3(0);
    }
}
