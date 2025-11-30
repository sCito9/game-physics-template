//
// Created by tonib on 28.11.2025.
//

#ifndef RIGIDBODY_CUBE_H
#define RIGIDBODY_CUBE_H
#include <list>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>



class Rigidbody_Cube {

    public:

    Rigidbody_Cube(glm::vec3 position, glm::quat rotation, glm::vec3 linearVelocity, float x_length, float y_length, float z_length, float mass);
    glm::vec3 scale;
    glm::vec3 cm_pos;
    glm::vec3 cm_linearVelocity;
    glm::vec3 angularVelocity;
    float M;    //overall mass
    glm::mat3 I0_1;   //inverted initial Inertia tensor
    glm::mat3 I_1;  //inverted current Inertia tensor
    glm::quat r;    //rotation quaternion
    glm::mat3 rot;  //rotation matrix
    glm::vec3 L;    //angular momentum
    glm::vec3 q;    //torque
    glm::vec3 F;    //external Forces
    std::list<glm::vec3> ForcePosQueue;
    std::list<glm::vec3> ForceDirQueue;
};



#endif //RIGIDBODY_CUBE_H
