#include "Scene.h"
#include <imgui.h>
#include "Punkt.h"
#include "Scene2.h"
#include "util/CollisionDetection.h"

void Scene::onDraw(Renderer &renderer)
{
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    for (Rigidbody_Cube *cube : cubes) {
        renderer.drawSphere(cube->cm_pos, 0.1f, {0, 0, 1, 1});
        renderer.drawCube(cube->cm_pos, cube->r, cube->scale, {1, 0, 0, .5f});
        for (Punkt point : cube->points) {
            renderer.drawSphere(point.worldPos, 0.1f, {1, 0, 0, 1});
        }
    }

    cameraMatrix = renderer.camera.viewMatrix;

    camPos = renderer.camera.position;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);

    //debug:
}

glm::mat4 getWorldMatrixFromCube(Rigidbody_Cube* cube) {
    glm::mat4 rotationMatrix = glm::toMat4(cube->r);
    glm::mat4 scaleMarix = glm::scale(glm::mat4(1.0f), cube->scale);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), cube->cm_pos);
    return translationMatrix * rotationMatrix * scaleMarix;
}

void Scene::simStep(float timeStep) {
    for (Rigidbody_Cube *cube : cubes) {
        for (Rigidbody_Cube *otherCube : cubes) {
            if (cube != otherCube) {
                glm::mat4 a = getWorldMatrixFromCube(cube);
                glm::mat4 b = getWorldMatrixFromCube(otherCube);
                CollisionInfo collisionInfo = collisionTools::checkCollisionSAT(a, b);

            }
        }
    }
    for (Rigidbody_Cube *cube : cubes) {
        assignPointForces(cube);
        cube->F = getExternalForces(cube);
        cube->q = getTorque(cube);

        cube->cm_pos += timeStep * cube->cm_linearVelocity;
        cube->cm_linearVelocity += timeStep * cube->F / cube->M;

        cube->r += timeStep/2 * glm::quat(0, cube->angularVelocity) * cube->r;
        cube->r = glm::normalize(cube->r);
        cube->rot = glm::toMat3(cube->r);

        cube->L += timeStep * cube->q;

        cube->I_1 = cube->rot * cube->I0_1 * glm::transpose(cube->rot);

        cube->angularVelocity = cube->I_1 * cube->L;

        for (int i = 0; i < 8; i++) {
            cube->points[i].worldPos = cube->cm_pos + (cube->rot * cube->local_vertices_pos[i]);
            cube->points[i].worldVel = cube->cm_linearVelocity + glm::cross(cube->angularVelocity, cube->local_vertices_pos[i]);
        }
    }
}

float stärke = .1f;
void Scene::assignPointForces(Rigidbody_Cube *cube) {
    for (int i = 0; i < 8; i++) {
        cube->points[i].currForce = glm::vec3(0, 0, 0);

        //cube->points[i].currForce = glm::vec3(0, 0, -9.81f);
        if (ImGui::IsKeyPressed(ImGuiKey_W)) {
            cube->points[i].currForce += glm::normalize(up) * stärke;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_S)) {
            cube->points[i].currForce += glm::normalize(up) * -stärke;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_A)) {
            cube->points[i].currForce += glm::normalize(right) * -stärke;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_D)) {
            cube->points[i].currForce += glm::normalize(right) * stärke;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_E)) {
            glm::vec3 temp = cube->points[i].worldPos - cube->cm_pos;
            if (glm::dot(temp, up) > 0) {
                cube->points[i].currForce += glm::normalize(right) * stärke;
            }
            else {
                cube->points[i].currForce += glm::normalize(right) * -stärke;
            }
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
            glm::vec3 temp = cube->points[i].worldPos - cube->cm_pos;
            if (glm::dot(temp, up) > 0) {
                cube->points[i].currForce += glm::normalize(right) * -stärke;
            }
            else {
                cube->points[i].currForce += glm::normalize(right) * stärke;
            }
        }
    }
}

bool initial = true;
glm::vec3 Scene::getExternalForces(Rigidbody_Cube* cube) {
    glm::vec3 force = glm::vec3(0, 0, 0);
    if (initial) {
        force += glm::vec3(1, 1, 0);
    }

    for (int i = 0; i < 8; i++) {
        force += cube->points[i].currForce;
    }

    return force;
}

glm::vec3 Scene::getTorque(Rigidbody_Cube* cube) {
    glm::vec3 torque = glm::vec3(0, 0, 0);
    if (initial) {
        initial = false;
        torque += glm::cross(glm::vec3{0.3, 0.5, 0.25}, glm::vec3{1, 1, 0});
    }

    for (int i = 0; i < 8; i++) {
        torque += glm::cross(cube->local_vertices_pos[i], cube->points[i].currForce);
    }

    return torque;
}



