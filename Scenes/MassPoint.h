//
// Created by tonib on 08.11.2025.
//

#ifndef MASSPOINT_H
#define MASSPOINT_H

#include <array>

#include "glm/vec3.hpp"


class MassPoint {
    public:
        float mass;
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        glm::vec3 force;

        MassPoint(float initMass, glm::vec3 initPos, glm::vec3 initVel, glm::vec3 initAcc, glm::vec3 initForce);
        void resetForces();
};



#endif //MASSPOINT_H
