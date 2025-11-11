#include "Scene3.h"

void Scene3::onDraw(Renderer& renderer){
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    renderer.drawSphere(mp0.x,
        /*radius*/ 0.1f,
        /*color*/ glm::vec4(1, 0, 0, 1));
        
    renderer.drawSphere(mp1.x,
        /*radius*/ 0.1f,
        /*color*/ glm::vec4(0, 1, 0, 1));

    renderer.drawLine(mp0.x, mp1.x, glm::vec4(1, 0.65f, 0, 1));
}

void Scene3::simulateStep()
{
    float realtimeDt = ImGui::GetIO().DeltaTime;

    cur_Δt += realtimeDt;
    for (; cur_Δt >= Δt; cur_Δt -= Δt) {
        calculateMidpointStep(&mp0, &mp1, &spr, Δt);
    }
}

void Scene3::onGUI()
{
    ImGui::SliderInt("delta t in μs", &Δt_μs, 500, 500000);
    Δt = ((float) Δt_μs) / 1000000.0f;
}

void Scene3::init() {
    mp0.mass = 10.f;
    mp0.x = glm::vec3(0.f, 0.f, 0.f);
    mp0.v = glm::vec3(-1.f, 0.f, 0.f);

    mp1.mass = 10.f;
    mp1.x = glm::vec3(0.f, 2.f, 0.f);
    mp1.v = glm::vec3(1.f, 0.f, 0.f);

    printMasspoints(mp0, mp1, "inital values:");

    spr.restLen = 1;
    spr.curLen = spr.restLen;
    spr.stiffness = 40.f;
}

void Scene3::printMasspoints(massPoint mp0, massPoint mp1, const char* headlineText)
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

void Scene3::calculateEulerStep(massPoint* mp0, massPoint* mp1, spring* spr, float h) {
    glm::vec3 F01 = ((-spr->stiffness / spr->curLen) * (spr->curLen - spr->restLen)) * (mp0->x - mp1->x);
    glm::vec3 F10 = -F01;

    mp0->v = mp0->v + h * (F01/mp0->mass);
    mp1->v = mp1->v + h * (F10/mp1->mass);

    mp0->x = mp0->x + h * mp0->v;
    mp1->x = mp1->x + h * mp1->v;

    spr->curLen = glm::length(mp0->x - mp1->x);
}

void Scene3::calculateMidpointStep(massPoint* mp0, massPoint* mp1, spring* spr, float h) {
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
