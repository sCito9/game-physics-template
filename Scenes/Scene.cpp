#include "Scene.h"
#include <imgui.h>
#include "Scene2.h"
#include "util/CollisionDetection.h"

void Scene::onDraw(Renderer &renderer)
{
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    for (Rigidbody_Cube *cube : cubes) {
        renderer.drawSphere(cube->cm_pos, 0.1f, {0, 0, 1, 1});
        renderer.drawCube(cube->cm_pos, cube->r, cube->scale, {1, 0, 0, 1.f});
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
    //collision und forceberechnung
    detectedCollisions.clear();
    for (Rigidbody_Cube *cube : cubes) {
        for (Rigidbody_Cube *otherCube : cubes) {
            if (cube != otherCube) {
                glm::mat4 a = getWorldMatrixFromCube(cube);
                glm::mat4 b = getWorldMatrixFromCube(otherCube);
                CollisionInfo collisionInfo = collisionTools::checkCollisionSAT(a, b);
                if (collisionInfo.isColliding) {
                    for (std::tuple<Rigidbody_Cube*, Rigidbody_Cube*> tuple : detectedCollisions) {     //weil ich grad alles mehrmals durchgehe, schaue ich, dass ich kollisionen die ich schon detektiert habe überspringe
                        Rigidbody_Cube* cube1 = std::get<0>(tuple);
                        Rigidbody_Cube* cube2 = std::get<1>(tuple);

                        if ((cube == cube1 && otherCube == cube2) || (cube == cube2 && otherCube == cube1)) {
                            goto A;
                        }
                    }
                    detectedCollisions.emplace_back(std::make_tuple(cube, otherCube));

                    glm::vec3 v_Cp_a = cube->cm_linearVelocity + glm::cross(cube->angularVelocity, (collisionInfo.collisionPointWorld - cube->cm_pos));
                    glm::vec3 v_Cp_b = otherCube->cm_linearVelocity + glm::cross(otherCube->angularVelocity, (collisionInfo.collisionPointWorld - otherCube->cm_pos));
                    glm::vec3 v_rel = v_Cp_a - v_Cp_b;
                    if (glm::dot(v_rel, collisionInfo.normalWorld) <= 0) {
                        glm::vec3 x_a = collisionInfo.collisionPointWorld - cube->cm_pos;
                        glm::vec3 x_b = collisionInfo.collisionPointWorld - otherCube->cm_pos;
                        float J = -(1 + collisionElasticity) * glm::dot(v_rel, collisionInfo.normalWorld);
                        J = J/(1/cube->M + 1/otherCube->M + glm::dot((cube->I_1 * glm::cross((glm::cross(x_a, collisionInfo.normalWorld)), x_a)), collisionInfo.normalWorld) + glm::dot((otherCube->I_1 * glm::cross(glm::cross(x_b, collisionInfo.normalWorld), x_b)), collisionInfo.normalWorld));

                        cube->cm_linearVelocity += J/cube->M * collisionInfo.normalWorld;
                        otherCube->cm_linearVelocity -= J/otherCube->M * collisionInfo.normalWorld;

                        cube->L += glm::cross(x_a, J * collisionInfo.normalWorld);
                        otherCube->L -= glm::cross(x_b, J * collisionInfo.normalWorld);
                    }
                }
                A:
            }
        }
    }

    for (Rigidbody_Cube *cube : cubes) {
        //apply forces
        cube->F = glm::vec3(0, 0, 0);
        cube->q = glm::vec3(0, 0, 0);
        while (!cube->ForcePosQueue.empty()) {
            cube->F += cube->ForceDirQueue.front();
            cube->q += glm::cross(cube->ForcePosQueue.front(), cube->ForceDirQueue.front());

            cube->ForcePosQueue.pop_front();
            cube->ForceDirQueue.pop_front();
        }

        cube->cm_pos += timeStep * cube->cm_linearVelocity;
        cube->cm_linearVelocity += timeStep * cube->F / cube->M;

        cube->r += timeStep/2.f * glm::quat(0, cube->angularVelocity) * cube->r;
        cube->r = glm::normalize(cube->r);
        cube->rot = glm::toMat3(cube->r);

        cube->L += timeStep * cube->q;

        cube->I_1 = cube->rot * cube->I0_1 * glm::transpose(cube->rot);

        cube->angularVelocity = cube->I_1 * cube->L;

    }
}

void Scene::addForce(Rigidbody_Cube *cube, glm::vec3 pos, glm::vec3 force) {
    cube->ForcePosQueue.push_back(pos);
    cube->ForceDirQueue.push_back(force);
}