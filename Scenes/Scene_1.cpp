//
// Created by tonib on 07.11.2025.
//
#include "Scene_1.h"

#include "MassPoint.h"
#include "Spring.h"
#include "glm/gtx/string_cast.hpp"

void Scene_1::init() {
    MassPoint *point0 = new MassPoint(10, {0, 0, 0}, {-1, 0, 0}, {0, 0, 0}, {0, 0, 0});
    MassPoint *point1 = new MassPoint(10, {0, 2, 0}, {1, 0, 0}, {0, 0, 0}, {0, 0, 0});

    Spring *s = new Spring(40, 1, point0, point1);

    MassPoint *point02 = new MassPoint(10, {0, 0, 0}, {-1, 0, 0}, {0, 0, 0}, {0, 0, 0});
    MassPoint *point12 = new MassPoint(10, {0, 2, 0}, {1, 0, 0}, {0, 0, 0}, {0, 0, 0});

    Spring *s2 = new Spring(40, 1, point02, point12);

    printf("Masspoint0\n");
    printf("position: %s\n", glm::to_string(point0->position).c_str());
    printf("velocity: %s\n", glm::to_string(point0->velocity).c_str());
    printf("Masspoint1\n");
    printf("position: %s\n", glm::to_string(point1->position).c_str());
    printf("velocity: %s\n", glm::to_string(point1->velocity).c_str());

    printf("\n");
    printf("After 0.1s using Midpoint Method:\n");
    calcMidpointStep(0.1, s);

    printf("Masspoint0\n");
    printf("position: %s\n", glm::to_string(point0->position).c_str());
    printf("velocity: %s\n", glm::to_string(point0->velocity).c_str());
    printf("Masspoint1\n");
    printf("position: %s\n", glm::to_string(point1->position).c_str());
    printf("velocity: %s\n", glm::to_string(point1->velocity).c_str());

    printf("\n");
    printf("After 0.1s using Euler Method:\n");
    calcEulerStep(0.1, s2);

    printf("Masspoint0\n");
    printf("position: %s\n", glm::to_string(point02->position).c_str());
    printf("velocity: %s\n", glm::to_string(point02->velocity).c_str());
    printf("Masspoint1\n");
    printf("position: %s\n", glm::to_string(point12->position).c_str());
    printf("velocity: %s\n", glm::to_string(point12->velocity).c_str());
}

void Scene_1::calcEulerStep(float timeStep, Spring *s) {
    s->point1->position += timeStep*s->point1->velocity;
    s->point2->position += timeStep*s->point2->velocity;

    s->point1->velocity += timeStep * s->point1->acceleration;
    s->point2->velocity += timeStep * s->point2->acceleration;

    s->point1->acceleration = s->point1->force / s->point1->mass;
    s->point2->acceleration = s->point2->force / s->point2->mass;

    glm::vec3 force1_2 = -1 * (s->stiffness) * (s->getCurrentLength() - s->restLength) * (1/s->getCurrentLength() * (s->point1->position - s->point2->position));
    s->point1->force += force1_2;
    glm::vec3 force2_1 = -force1_2;
    s->point2->force += force2_1;
}

void Scene_1::calcMidpointStep(float timeStep, Spring *s) {
    glm::vec3 x1tmp = s->point1->position + timeStep/2 * s->point1->velocity;
    glm::vec3 x2tmp = s->point2->position + timeStep/2 * s->point2->velocity;

    glm::vec3 force1_2 = -1 * (s->stiffness) * (s->getCurrentLength() - s->restLength) * (1/s->getCurrentLength() * (s->point1->position - s->point2->position));
    s->point1->force += force1_2;
    glm::vec3 force2_1 = -force1_2;
    s->point2->force += force2_1;

    s->point1->acceleration = s->point1->force / s->point1->mass;
    s->point2->acceleration = s->point2->force / s->point2->mass;

    glm::vec3 v1tmp = s->point1->velocity + timeStep/2 * s->point1->acceleration;
    glm::vec3 v2tmp = s->point2->velocity + timeStep/2 * s->point2->acceleration;

    s->point1->position += timeStep * v1tmp;
    s->point2->position += timeStep * v2tmp;

    glm::vec3 force1_2tmp = -1 * (s->stiffness) * (glm::length(x1tmp - x2tmp) - s->restLength) * (1/glm::length(x1tmp - x2tmp) * (s->point1->position - s->point2->position));
    glm::vec3 a1tmp = force1_2tmp/s->point1->mass;
    glm::vec3 a2tmp = -force1_2tmp/s->point2->mass;

    s->point1->velocity += timeStep * a1tmp;
    s->point2->velocity += timeStep * a2tmp;
}
