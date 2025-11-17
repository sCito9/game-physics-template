#include "Scene2.h"

void Scene2::onDraw(Renderer& renderer){
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    renderer.drawSphere(mp0.x,
        /*radius*/ 0.1f,
        /*color*/ glm::vec4(1, 0, 0, 1));
        
    renderer.drawSphere(mp1.x,
        /*radius*/ 0.1f,
        /*color*/ glm::vec4(0, 1, 0, 1));

    renderer.drawLine(mp0.x, mp1.x, glm::vec4(1, 0.65f, 0, 1));
}

void Scene2::simulateStep()
{
    float realtimeDt = ImGui::GetIO().DeltaTime;

    cur_Δt += realtimeDt;
    for (; cur_Δt >= Δt; cur_Δt -= Δt) {
        calculateEulerStep(&mp0, &mp1, &spr, Δt);
    }
}

void Scene2::onGUI()
{
    ImGui::SliderInt("delta t in μs", &Δt_μs, 500, 5000000);
    Δt = ((float) Δt_μs) / 1000000.0f;
}

void Scene2::init() {
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

void Scene2::printMasspoints(massPoint mp0, massPoint mp1, const char* headlineText)
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

void Scene2::calculateEulerStep(massPoint* mp0, massPoint* mp1, spring* spr, float h) {
    glm::vec3 F01 = ((-spr->stiffness / spr->curLen) * (spr->curLen - spr->restLen)) * (mp0->x - mp1->x);
    glm::vec3 F10 = -F01;

    mp0->x = mp0->x + h * mp0->v;
    mp1->x = mp1->x + h * mp1->v;

    mp0->v = mp0->v + h * (F01/mp0->mass);
    mp1->v = mp1->v + h * (F10/mp1->mass);


    spr->curLen = glm::length(mp0->x - mp1->x);
}