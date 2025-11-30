//
// Created by tonib on 28.11.2025.
//

#ifndef PUNKT_H
#define PUNKT_H
#include "glm/vec3.hpp"


class Punkt {

    public:
    glm::vec3 worldPos;
    glm::vec3 worldVel;
    glm::vec3 currForce;
    float mass;

    Punkt(glm::vec3 worldPosition, glm::vec3 worldVelocity, float mass);
    Punkt();
};



#endif //PUNKT_H
