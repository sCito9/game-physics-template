//
// Created by tonib on 28.11.2025.
//

#include "Punkt.h"

Punkt::Punkt(glm::vec3 worldPosition, glm::vec3 worldVelocity, float mass) {
    worldPos = worldPosition;
    worldVel = worldVelocity;
    currForce = glm::vec3(0, 0, 0);
    this->mass = mass;
}

Punkt::Punkt() {
    worldPos = glm::vec3(0, 0, 0);
    worldVel = glm::vec3(0, 0, 0);
    currForce = glm::vec3(0, 0, 0);
    this->mass = 0;
}
