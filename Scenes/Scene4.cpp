#include "Scene4.h"

void Scene4::onDraw(Renderer& renderer){
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    for (int i = 0; i < nMasspoints; i++) {
        renderer.drawSphere(massPoints[i].x,
            /*radius*/ 0.1f,
            /*color*/ glm::vec4(1, 0, 0, 1));
    }

    for (int i = 0; i < nSprings; i++)
    {
        renderer.drawLine(massPoints[springs[i].mp0_index].x, massPoints[springs[i].mp1_index].x, glm::vec4(1, 0.65f, 0, 1));
    }
}

void Scene4::simulateStep()
{
    float realtimeDt = ImGui::GetIO().DeltaTime;

    cur_Δt += realtimeDt;
    for (; cur_Δt >= Δt; cur_Δt -= Δt) {
        simulateAllMasspoints(Δt);
    }
}

void Scene4::simulateAllMasspoints(float h) {
    for (int i = 0; i < nSprings; i++) {
        (this->*stepFunc)(&massPoints[springs[i].mp0_index], &massPoints[springs[i].mp1_index], &springs[i], h);
    }
}

void Scene4::onGUI()
{
    ImGui::SliderInt("delta t in μs", &Δt_μs, 500, 50000);
    Δt = ((float) Δt_μs) / 1000000.0f;
}

void Scene4::init() {
    stepFunc = calculateMidpointStep;

    massPoints = (massPoint*) malloc(nMasspoints * sizeof(struct massPoint));
    if (massPoints < 0) {
        std::cout << "Error mallocing for " << nMasspoints << "masspoints" << std::endl;
    }
}

void Scene4::printMasspoints(massPoint* masspoints, int nMasspoints, const char* headlineText)
{
    std::cout << std::endl;

    if (std::strlen(headlineText) > 0) {
        std::cout << headlineText << std::endl;
    }

    for (int i = 0; i < nMasspoints; i++)
    {
        std::cout << "Masspoint " << i << std::endl;
        std::cout << "position: " << masspoints[i].x << std::endl;
        std::cout << "velocity: " << masspoints[i].v << std::endl; 
    }
}

void Scene4::calculateEulerStep(massPoint* mp0, massPoint* mp1, spring* spr, float h) {
    glm::vec3 F01 = ((-spr->stiffness / spr->curLen) * (spr->curLen - spr->restLen)) * (mp0->x - mp1->x);
    glm::vec3 F10 = -F01;

    mp0->v = mp0->v + h * (F01/mp0->mass);
    mp1->v = mp1->v + h * (F10/mp1->mass);

    mp0->x = mp0->x + h * mp0->v;
    mp1->x = mp1->x + h * mp1->v;

    spr->curLen = glm::length(mp0->x - mp1->x);
}

void Scene4::calculateMidpointStep(massPoint* mp0, massPoint* mp1, spring* spr, float h) {
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
