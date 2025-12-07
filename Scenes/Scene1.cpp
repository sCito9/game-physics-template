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
    cubes.front()->ForcePosQueue.push_back(glm::vec3(0.3f, 0.5f, 0.25f));
    cubes.front()->ForceDirQueue.push_back(glm::vec3(1, 1, 0));

    printf("Positions:\n");
    printf("Center of mass: x: %f, y: %f, z: %f\n", cubes.front()->cm_pos.x, cubes.front()->cm_pos.y, cubes.front()->cm_pos.z);
    printf("Point_x: x: %f, y: %f, z: %f\n", getPoint_x_pos(cubes.front()).x, getPoint_x_pos(cubes.front()).y, getPoint_x_pos(cubes.front()).z);

    printf("\n");

    printf("Velocity:\n");
    printf("Center of mass: x: %f, y: %f, z: %f\n", cubes.front()->cm_linearVelocity.x, cubes.front()->cm_linearVelocity.y, cubes.front()->cm_linearVelocity.z);
    printf("Point_x: x: %f, y: %f, z: %f\n", getPoint_x_vel(cubes.front()).x, getPoint_x_vel(cubes.front()).y, getPoint_x_vel(cubes.front()).z);

    std::cout <<cubes.front()->I0_1[0] << cubes.front()->I0_1[1] << cubes.front()->I0_1[2] << std::endl;
    std::cout <<cubes.front()->r.w <<","<< cubes.front()->r.x <<","<< cubes.front()->r.y <<","<< cubes.front()->r.z << std::endl;

    printf("\n");
    printf("Simulating........\n");
    simStep(2);
    printf("\n");

    printf("Positions:\n");
    printf("Center of mass: x: %f, y: %f, z: %f\n", cubes.front()->cm_pos.x, cubes.front()->cm_pos.y, cubes.front()->cm_pos.z);
    printf("Point_x: x: %f, y: %f, z: %f\n", getPoint_x_pos(cubes.front()).x, getPoint_x_pos(cubes.front()).y, getPoint_x_pos(cubes.front()).z);

    printf("\n");

    printf("Velocity:\n");
    printf("Center of mass: x: %f, y: %f, z: %f\n", cubes.front()->cm_linearVelocity.x, cubes.front()->cm_linearVelocity.y, cubes.front()->cm_linearVelocity.z);
    printf("Point_x: x: %f, y: %f, z: %f\n", getPoint_x_vel(cubes.front()).x, getPoint_x_vel(cubes.front()).y, getPoint_x_vel(cubes.front()).z);
}

glm::vec3 Scene1::getPoint_x_pos(Rigidbody_Cube* cube) {
    return cube->cm_pos + (cube->rot * glm::vec3(0.3f, 0.5f, 0.25f));
}

glm::vec3 Scene1::getPoint_x_vel(Rigidbody_Cube* cube) {
    return cube->cm_linearVelocity + glm::cross(cube->angularVelocity, glm::vec3(0.3f, 0.5f, 0.25f));
}
