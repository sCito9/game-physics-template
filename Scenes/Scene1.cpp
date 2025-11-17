#include "Scene1.h"

void Scene1::init() {
    massPoint mp0_euler;
    mp0_euler.mass = 10.f;
    mp0_euler.x = glm::vec3(0.f, 0.f, 0.f);
    mp0_euler.v = glm::vec3(-1.f, 0.f, 0.f);

    massPoint mp0_midpoint;
    mp0_midpoint.mass = 10.f;
    mp0_midpoint.x = glm::vec3(0.f, 0.f, 0.f);
    mp0_midpoint.v = glm::vec3(-1.f, 0.f, 0.f);

    massPoint mp1_euler;
    mp1_euler.mass = 10.f;
    mp1_euler.x = glm::vec3(0.f, 2.f, 0.f);
    mp1_euler.v = glm::vec3(1.f, 0.f, 0.f);

    massPoint mp1_midpoint;
    mp1_midpoint.mass = 10.f;
    mp1_midpoint.x = glm::vec3(0.f, 2.f, 0.f);
    mp1_midpoint.v = glm::vec3(1.f, 0.f, 0.f);

    printMasspoints(mp0_euler, mp1_euler, "inital values:");

    spring spr_euler;
    spr_euler.restLen = 1;
    spr_euler.curLen = spr_euler.restLen;
    spr_euler.stiffness = 40.f;
    
    spring spr_midpoint;
    spr_midpoint.restLen = 1;
    spr_midpoint.curLen = spr_midpoint.restLen;
    spr_midpoint.stiffness = 40.f;

    calculateEulerStep(&mp0_euler, &mp1_euler, &spr_euler, 0.1f);
    printMasspoints(mp0_euler, mp1_euler, "After 0.1s using Euler Method:");

    calculateMidpointStep(&mp0_midpoint, &mp1_midpoint, &spr_midpoint, 0.1f);
    printMasspoints(mp0_midpoint, mp1_midpoint, "After 0.1s using Midpoint Method:");
}

void Scene1::printMasspoints(massPoint mp0, massPoint mp1, const char* headlineText)
{
    std::cout << std::endl;

    if (std::strlen(headlineText) > 0) {
        std::cout << headlineText << std::endl;
    }

    std::cout << "Masspoint 0" << std::endl;
    std::cout << "position: " << mp0.x << std::endl;
    std::cout << "velocity: " << mp0.v << std::endl;

    std::cout << "Masspoint 1" << std::endl;
    std::cout << "position: " << mp1.x << std::endl;
    std::cout << "velocity: " << mp1.v << std::endl;
}

void Scene1::calculateEulerStep(massPoint* mp0, massPoint* mp1, spring* spr, float h) {
    // technically irrelevant for only 1 step
    glm::vec3 F01 = ((-spr->stiffness / spr->curLen) * (spr->curLen - spr->restLen)) * (mp0->x - mp1->x);
    glm::vec3 F10 = -F01;

    mp0->x = mp0->x + h * mp0->v;
    mp1->x = mp1->x + h * mp1->v;

    // technically irrelevant for only 1 step
    mp0->v = mp0->v + h * (F01/mp0->mass);
    mp1->v = mp1->v + h * (F10/mp1->mass);

    spr->curLen = glm::length(mp0->x - mp1->x);
}

void Scene1::calculateMidpointStep(massPoint* mp0, massPoint* mp1, spring* spr, float h) {
    massPoint mp0_mid;
    mp0_mid.mass = mp0->mass;
    mp0_mid.x = mp0->x;
    mp0_mid.v = mp0->v;
    
    massPoint mp1_mid;
    mp1_mid.mass = mp1->mass;
    mp1_mid.x = mp1->x;
    mp1_mid.v = mp1->v;

    calculateEulerStep(&mp0_mid, &mp1_mid, spr, h/2);

    glm::vec3 F01 = ((-spr->stiffness / spr->curLen) * (spr->curLen - spr->restLen)) * (mp0->x - mp1->x);
    glm::vec3 F10 = -F01;

    mp0->x = mp0->x + h * mp0_mid.v;
    mp1->x = mp1->x + h * mp1_mid.v;

    // technically irrelevant for only 1 step
    mp0->v = mp0->v + h * (F01/mp0->mass);
    mp1->v = mp1->v + h * (F10/mp1->mass);
    
    spr->curLen = glm::length(mp0->x - mp1->x);
}

