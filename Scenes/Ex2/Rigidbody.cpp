#include "Rigidbody.h"
#include "glm/gtx/quaternion.hpp"

CuboidRigidbody initializeCuboidRb(glm::vec3 x_cm_0, glm::vec3 dimensions, float M, glm::vec3 rotation_0, glm::vec3 v_cm_0, glm::vec3 L_0) {
    auto rb = CuboidRigidbody {0};

    rb.M = M;

    rb.x_cm = x_cm_0;

    /* x_i unused, therefore omitted */

    float w = dimensions.x;
    float d = dimensions.y;
    float h = dimensions.z;
    float konst = 12.0f / M;
    rb.I[0][0] = konst / (h*h + d*d);
    rb.I[1][1] = konst / (w*w + h*h);
    rb.I[2][2] = konst / (w*w + d*d);

    rb.v_cm = v_cm_0;

    rb.r = glm::quat(rotation_0);
    normalize(rb.r);

    rb.L = L_0;

    glm::mat3x3 Rot_r = toMat3(rb.r);
    glm::mat3x3 I_cur = Rot_r * rb.I * transpose(Rot_r);

    rb.w = I_cur * L_0;

    return rb;
}

void printRb(CuboidRigidbody* rb) {
    std::cout << "\nRigidbody:\nCenter of mass: " << rb->x_cm << " \nv_cm: " << rb->v_cm << "\nw: " << rb->w
        << "\nI^-1: (" << rb->I[0] << rb->I[1] << rb->I[2] << ")\nr:" << rb->r << std::endl;
}

void simulateCuboidRb(CuboidRigidbody *rb, float h, glm::vec3 *f_i, glm::vec3 *f_i_x, uint8_t nForces) {
    // External forces
    auto F = glm::vec3(0);
    auto q = glm::vec3(0);

    for (int i = 0; i < nForces; i++) {
        F += f_i[i];
        q += cross(f_i_x[i] - rb->x_cm, f_i[i]);
    }

    // Euler step
    rb->x_cm += h * rb->v_cm;
    rb->v_cm += h * F / rb->M;

    rb->r += h / 2.0f * glm::quat(0, rb->w) * rb->r;
    glm::normalize(rb->r);

    rb->L += h * q;

    // I^-1 = Rot_r * (I_0)^-1 * Rot_r^T
    glm::mat3x3 Rot_r = toMat3(rb->r);
    glm::mat3x3 I_cur = Rot_r * rb->I * transpose(Rot_r);

    rb->w = I_cur * rb->L;
}

void simulateCuboidRb(CuboidRigidbody *rb, float h, glm::vec3 *f_i, uint8_t nForces) {
    auto f_x_i = new glm::vec3[]{rb->x_cm, rb->x_cm, rb->x_cm};
    simulateCuboidRb(rb, h, f_i, f_x_i, nForces);
}

glm::mat4 getWorldFromObj(CuboidRigidbody *rb, glm::vec3 dimensions) {
    glm::mat4 rotationMatrix = glm::toMat4(rb->r);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), dimensions);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1), rb->x_cm);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

float calculateImpulseFromCollision(glm::vec3 v_rel, glm::vec3 n, CuboidRigidbody *rb_a, CuboidRigidbody *rb_b) {
    const float c = 1;
    float numerator = -(1 + c) * dot(v_rel, n);
    glm::mat3x3 Rot_r = toMat3(rb_a->r);
    glm::mat3x3 I_a = Rot_r * rb_a->I * transpose(Rot_r);
    Rot_r = toMat3(rb_b->r);
    glm::mat3x3 I_b = Rot_r * rb_b->I * transpose(Rot_r);
    float denominator = 1/rb_a->M + 1/rb_b->M + dot((cross(I_a * cross(rb_a->x_cm, n), rb_a->x_cm)
            + cross(I_b * cross(rb_b->x_cm, n), rb_b->x_cm)), n);
    return numerator / denominator;
}
