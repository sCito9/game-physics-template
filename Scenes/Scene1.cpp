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

    std::string line; 
    std::getline(std::cin, line);
    int nSteps = std::stoi(line);

    for (int i = 0; i < nSteps; i++) {
        calculateEulerStep(&mp0_euler, &mp1_euler, &spr_euler, 0.1f);
        calculateMidpointStep(&mp0_midpoint, &mp1_midpoint, &spr_midpoint, 0.1f);
    }

    printMasspoints(mp0_euler, mp1_euler, "End after using Euler Method:");
    printMasspoints(mp0_midpoint, mp1_midpoint, "End after using Midpoint Method:");
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
    // F_ij = -k(curLen-initLen)*(x_i - x_j)/l

    glm::vec3 F01 = ((-spr->stiffness / spr->curLen) * (spr->curLen - spr->restLen)) * (mp0->x - mp1->x);
    glm::vec3 F10 = -F01;

    mp0->v = mp0->v + h * (F01/mp0->mass);
    mp1->v = mp1->v + h * (F10/mp1->mass);

    mp0->x = mp0->x + h * mp0->v;
    mp1->x = mp1->x + h * mp1->v;

    spr->curLen = glm::length(mp0->x - mp1->x);
}

void Scene1::calculateMidpointStep(massPoint* mp0, massPoint* mp1, spring* spr, float h) {
    massPoint mp0_½h;
    mp0_½h.mass = mp0->mass;
    mp0_½h.x = mp0->x;
    mp0_½h.v = mp0->v;
    
    massPoint mp1_½h;
    mp1_½h.mass = mp1->mass;
    mp1_½h.x = mp1->x;
    mp1_½h.v = mp1->v;

    glm::vec3 F01 = ((-spr->stiffness / spr->curLen) * (spr->curLen - spr->restLen)) * (mp0->x - mp1->x);
    glm::vec3 F10 = -F01;

    calculateEulerStep(&mp0_½h, &mp1_½h, spr, h/2);

    F01 = ((-spr->stiffness / spr->curLen) * (spr->curLen - spr->restLen)) * (mp0->x - mp1->x);
    F10 = -F01;

    mp0->v = mp0->v + h * (F01/mp0->mass);
    mp1->v = mp1->v + h * (F10/mp1->mass);
    
    mp0->x = mp0->x + h * mp0_½h.v;
    mp1->x = mp1->x + h * mp1_½h.v;
    
    spr->curLen = glm::length(mp0->x - mp1->x);
}

