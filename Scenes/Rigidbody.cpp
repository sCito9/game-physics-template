//
// Created by felix on 12.12.2025.
//

#include "Rigidbody.h"

#include <glm/gtx/quaternion.hpp>

#include "util/CollisionDetection.h"

glm::quat multiplication(glm::quat q1, glm::quat q2)
{
    float s1 = q1.w;
    float s2 = q2.w;
    glm::vec3 v1 = glm::vec3(q1.x, q1.y, q1.z);
    glm::vec3 v2 = glm::vec3(q2.x, q2.y, q2.z);

    return glm::quat(s1 * s2 - glm::dot(v1, v2), s1 * v2 + s2 * v1 + glm::cross(v1, v2));
}


void simulateCube(Cube* obj, glm::vec3 F, glm::vec3 force_position, bool gravity, float step_size)
{
    //add gravity
    glm::vec3 F_gravity = F;
    if (gravity) F_gravity += obj->M * glm::vec3(0.f, 0.f, -9.81f);

    //torque
    glm::vec3 q = glm::vec3(glm::cross(
        glm::vec3(force_position[0], force_position[1], force_position[2]) - obj->position_cm,
        F));

    //update center of mass
    obj->position_cm += step_size * obj->velocity_cm;
    obj->velocity_cm += step_size * F_gravity / obj->M;

    //rotation
    glm::quat temp = step_size / 2.f * multiplication(obj->orientation, glm::quat(0.f, obj->w.x, obj->w.y, obj->w.z));
    obj->orientation = glm::quat(obj->orientation.w + temp.w, obj->orientation.x + temp.x, obj->orientation.y + temp.y,
                                 obj->orientation.z + temp.z);
    obj->orientation = glm::normalize(obj->orientation);

    //angular momentum
    obj->L += step_size * q;

    //update inverse tensor
    obj->I_inv = glm::mat3_cast(obj->orientation) * obj->I_body * glm::transpose(glm::mat3_cast(obj->orientation));

    //update angular velocity
    obj->w = obj->I_inv * obj->L;
}


glm::mat4 getWorldFromObj(Cube* cube)
{
    glm::mat4 rotationMatrix = glm::toMat4(cube->orientation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), cube->scale);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), cube->position_cm);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

void checkCollisions(std::vector<Cube*> cubes, const float c)
{
    for (int i = 0; i < cubes.size() - 1; ++i)
    {
        for (int j = i + 1; j < cubes.size(); ++j)
        {
            glm::mat4 world_1 = getWorldFromObj(cubes[i]);
            glm::mat4 world_2 = getWorldFromObj(cubes[j]);
            CollisionInfo hitInfo = collisionTools::checkCollisionSAT(world_1, world_2);
            if (hitInfo.isColliding)
            {
                calculateCollision(hitInfo, cubes[i], cubes[j], c);
            }
        }
    }
}

void calculateCollision(CollisionInfo hitInfo, Cube* cube_1, Cube* cube_2, const float c)
{
    hitInfo.normalWorld = glm::normalize(hitInfo.normalWorld);
    glm::vec3 w_a = cube_1->I_inv * cube_1->L;
    glm::vec3 w_b = cube_2->I_inv * cube_2->L;
    //flip sign of normal if necessary
    glm::vec3 pre_collision_relative_velocity = cube_1->velocity_cm +
        glm::cross(w_a, hitInfo.collisionPointWorld - cube_1->position_cm) - cube_2->velocity_cm - glm::cross(
            w_b, hitInfo.collisionPointWorld - cube_2->position_cm);

    float rel_normal_dot = glm::dot(pre_collision_relative_velocity, hitInfo.normalWorld);
    if (rel_normal_dot > 0.f) return;

    glm::vec3 fromAtoB = cube_2->position_cm - cube_1->position_cm;
    if (glm::dot(hitInfo.normalWorld, fromAtoB) < 0.f) hitInfo.normalWorld = -hitInfo.normalWorld;


    float J = (-(1 + c) * glm::dot(pre_collision_relative_velocity, hitInfo.normalWorld)) /
    ((1 / cube_1->M) + (1 / cube_2->M) + glm::dot(glm::cross(cube_1->I_inv * (glm::cross(
                                                                 hitInfo.collisionPointWorld - cube_1->position_cm,
                                                                 hitInfo.normalWorld)),
                                                             hitInfo.collisionPointWorld - cube_1->position_cm),
                                                  hitInfo.normalWorld)
        + glm::dot(glm::cross(cube_2->I_inv * (glm::cross(hitInfo.collisionPointWorld - cube_2->position_cm,
                                                          hitInfo.normalWorld)),
                              hitInfo.collisionPointWorld - cube_2->position_cm), hitInfo.normalWorld));


    cube_1->velocity_cm = cube_1->velocity_cm + J * hitInfo.normalWorld / cube_1->M;
    cube_1->L = cube_1->L + glm::cross(hitInfo.collisionPointWorld - cube_1->position_cm, J * hitInfo.normalWorld);


    cube_2->velocity_cm = cube_2->velocity_cm - J * hitInfo.normalWorld / cube_2->M;
    cube_2->L = cube_2->L - glm::cross(hitInfo.collisionPointWorld - cube_2->position_cm, J * hitInfo.normalWorld);
}

Cube initializeCube(glm::vec3 x_cm_0, glm::vec3 dimensions, float M,
                    glm::vec3 rotation_0, glm::vec3 v_cm_0, glm::vec3 L_0)
{
    auto rb = Cube{};

    rb.M = M;
    rb.scale = dimensions;

    rb.position_cm = x_cm_0;

    /* x_i unused, therefore omitted */

    float w = dimensions.x;
    float d = dimensions.y;
    float h = dimensions.z;
    float konst = 12.0f / M;
    rb.I_inv[0][0] = konst / (h * h + d * d);
    rb.I_inv[1][1] = konst / (w * w + h * h);
    rb.I_inv[2][2] = konst / (w * w + d * d);
    rb.I_body = rb.I_inv;

    rb.velocity_cm = v_cm_0;

    rb.orientation = glm::quat(rotation_0);
    glm::normalize(rb.orientation);

    rb.L = L_0;

    glm::mat3x3 Rot_r = glm::toMat3(rb.orientation);
    glm::mat3x3 I_cur = Rot_r * rb.I_inv * transpose(Rot_r);

    rb.w = I_cur * L_0;

    return rb;
}

