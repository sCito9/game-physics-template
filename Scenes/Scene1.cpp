//
// Created by tonib on 28.11.2025.
//

#include <glm/gtc/constants.hpp>

#include "Scene1.h"

void Scene1::init() {
    glm::mat3 initialRotation = glm::mat3(cos(glm::pi<float>() / 2), -sin(glm::pi<float>() / 2), 0,
                                        sin(glm::pi<float>() / 2), cos(glm::pi<float>() /2), 0,
                                        0, 0, 1);
    cubes.emplace_back(new Rigidbody_Cube({0, 0, 0}, glm::toQuat(initialRotation), {0, 0, 0}, 1.f, 0.6f, 0.5f, 2));
    printf("Positions:\n");
    for (int i = 0; i < 8; i++) {
        printf("Point %i: x: %f, y: %f, z: %f\n", i, cubes.front()->points[i].worldPos.x, cubes.front()->points[i].worldPos.y, cubes.front()->points[i].worldPos.z);
    }

    printf("\n");

    printf("Velocities:\n");
    for (int i = 0; i < 8; i++) {
        printf("Point %i: x: %f, y: %f, z: %f\n", i, cubes.front()->points[i].worldVel.x, cubes.front()->points[i].worldVel.y, cubes.front()->points[i].worldVel.z);
    }

    printf("\n");
    printf("Simulating........\n");
    simStep(2);
    printf("\n");

    printf("Positions:\n");
    for (int i = 0; i < 8; i++) {
        printf("Point %i: x: %f, y: %f, z: %f\n", i, cubes.front()->points[i].worldPos.x, cubes.front()->points[i].worldPos.y, cubes.front()->points[i].worldPos.z);
    }

    printf("\n");

    printf("Velocities:\n");
    for (int i = 0; i < 8; i++) {
        printf("Point %i: x: %f, y: %f, z: %f\n", i, cubes.front()->points[i].worldVel.x, cubes.front()->points[i].worldVel.y, cubes.front()->points[i].worldVel.z);
    }
}
