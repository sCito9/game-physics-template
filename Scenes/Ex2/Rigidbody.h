#pragma once

#include "Scenes/vec3_util.h"
#include "Scenes/Scene.h"

struct CuboidRigidbody {
    /// @brief Total mass M = Σ m_i
    float M;

    /// @brief Position of center of mass
    glm::vec3 x_cm;

    /// @brief (I_0)^-1 = Inverse of inertia tensor I_0
    glm::mat3x3 I;

    /// @brief Linear velocity of center of mass
    glm::vec3 v_cm;

    /// @brief Rotation quaternion
    glm::quat r;

    /// @brief Angular velocity
    glm::vec3 w;

    /// @brief Angular momentum
    glm::vec3 L;
};

inline std::ostream& operator << (std::ostream& os, const glm::quat& q) {
    os << "(s: " << q.w << ", ijk: " << q.x << ", " << q.y << ", " << q.z << ")";
    return os;
}

CuboidRigidbody initializeCuboidRb(glm::vec3 x_cm_0,
                                   glm::vec3 dimensions,
                                   float M,
                                   glm::vec3 rotation_0,
                                   glm::vec3 v_cm_0,
                                   glm::vec3 L_0);

void printRb(CuboidRigidbody* rb);

void simulateCuboidRb(CuboidRigidbody* rb, float h, glm::vec3 *f_i, glm::vec3 *f_i_x, uint8_t nForces);

void simulateCuboidRb(CuboidRigidbody* rb, float h, glm::vec3 *f_i, uint8_t nForces);

glm::mat4 getWorldFromObj(CuboidRigidbody* rb, glm::vec3 dimensions);

float calculateImpulseFromCollision(glm::vec3 v_rel, glm::vec3 n, CuboidRigidbody* rb_a, CuboidRigidbody* rb_b);
