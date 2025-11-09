//
// Created by tonib on 08.11.2025.
//

#include "Spring.h"

#include "glm/detail/func_geometric.inl"

Spring::Spring(float initStiffness, float initRestLength, MassPoint* initPoint1, MassPoint* initPoint2) {
    stiffness = initStiffness;
    restLength = initRestLength;
    point1 = initPoint1;
    point2 = initPoint2;
}

float Spring::getCurrentLength() {
    glm::vec3 temp = point1->position - point2->position;
    return glm::length(temp);
}

