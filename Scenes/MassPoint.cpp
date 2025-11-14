//
// Created by tonib on 08.11.2025.
//

#include "MassPoint.h"

MassPoint::MassPoint(float initMass, glm::vec3 initPos, glm::vec3 initVel, glm::vec3 initAcc, glm::vec3 initForce) {
    mass = initMass;
    position = initPos;
    velocity = initVel;
    acceleration = initAcc;
    force = initForce;
    tempPos = glm::vec3(0, 0, 0);
    tempPos2 = glm::vec3(0, 0, 0);
    tempAccel = glm::vec3(0, 0, 0);
}

void MassPoint::resetForces() {
    force = glm::vec3(0, 0, 0);
}

