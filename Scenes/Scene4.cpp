#include "Scene4.h"

void Scene4::onDraw(Renderer& renderer){
    renderer.drawQuad(
        glm::vec3(0, 0, floor),
        glm::quat(glm::vec3(0, 0, 0)), 
        glm::vec2(13), 
        glm::vec4(.68f, .85f, .9f, 1));

    for (int i = 0; i < nMasspoints; i++) {
        renderer.drawSphere(
            massPoints[i].x,
            /*radius*/ 0.1f,
            /*color (red)*/ glm::vec4(1, 0, 0, 1));
    }

    for (int i = 0; i < nSprings; i++)
    {
        renderer.drawLine(
                /*from*/ massPoints[springs[i].mp0_index].x,
                /*to*/ massPoints[springs[i].mp1_index].x,
                /*color(orange)*/ glm::vec4(1, 0.65f, 0, 1));
    }
}

void Scene4::onGUI()
{
    ImGui::InputFloat("delta t in s", &deltaT);
    ImGui::InputFloat("multiply stiffness", &global_stiffness_multiplier);

    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        paused = !paused;
    }

    if (ImGui::Selectable("Euler", selectedMethod == Euler)) {
        selectedMethod = Euler;
        simFunc = &Scene4::simulateSceneEuler;
    }
    if (ImGui::Selectable("Midpoint", selectedMethod == Midpoint)) {
        selectedMethod = Midpoint;
        simFunc = &Scene4::simulateSceneMidpoint;
    }

    ImGui::InputFloat3("external Force on all", (float*) &extF);
}

void Scene4::init() {
    std::cout << "Complex Sim" << std::endl;

    simFunc = &Scene4::simulateSceneEuler;

    massPoints[0] = {
        glm::vec3(0), glm::vec3(0.f, 0.f, -2.f), glm::vec3(0.f, 0.f, 0.f), 10.f
    };
    massPoints[1] = {
        glm::vec3(0), glm::vec3(-1.f, -1.f, -1.f), glm::vec3(0.f, 0.f, 0.f), 10.f
    };
    massPoints[2] = {
        glm::vec3(0), glm::vec3(1.f, -1.f, -1.f), glm::vec3(0.f, 0.f, 0.f), 10.f
    };
    massPoints[3] = {
        glm::vec3(0), glm::vec3(1.f, 1.f, -1.f), glm::vec3(0.f, 0.f, 0.f), 10.f
    };
    massPoints[4] = {
        glm::vec3(0), glm::vec3(-1.f, 1.f, -1.f), glm::vec3(0.f, 0.f, 0.f), 10.f
    };
    massPoints[5] = {
        glm::vec3(0), glm::vec3(-1.f, -1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), 10.f
    };
    massPoints[6] = {
        glm::vec3(0), glm::vec3(1.f, -1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), 10.f
    };
    massPoints[7] = {
        glm::vec3(0), glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), 10.f
    };
    massPoints[8] = {
        glm::vec3(0), glm::vec3(-1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), 10.f
    };
    massPoints[9] = {
        glm::vec3(0), glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f, 0.f, 0.f), 10.f
    };

    for (int i = 0; i < nMasspoints; i++) {
        massPointsMid[i].mass = massPoints[i].mass;
        massPointsMid[i].F = massPoints[i].F;
        massPointsMid[i].v = massPoints[i].v;
        massPointsMid[i].x = massPoints[i].x;
    }

    std::cout << "Masspoints loaded" << std::endl;

    for (int i = 0; i < 10; i++) {
        springs[i].mp0_index = i;
        springs[i].mp1_index = (i + 1) % 10;
    }

    for (int i = 0; i < 3; i++) {
        springs[i + 10].mp0_index = 0;
        springs[i + 10].mp1_index = i + 2;
    }

    for (int i = 0; i < 3; i++) {
        springs[i + 13].mp0_index = 9;
        springs[i + 13].mp1_index = i + 5;
    }

    for (int i = 0; i < 6; i++) {
        springs[i + 16].mp0_index = 1;
        springs[i + 16].mp1_index = i + 2;
    }

    for (int i = 0; i < 5; i++) {
        springs[i + 22].mp0_index = 2;
        springs[i + 22].mp1_index = i + 4;
    }

    for (int i = 0; i < 4; i++) {
        springs[i + 27].mp0_index = 3;
        springs[i + 27].mp1_index = i + 5;
    }

    for (int i = 0; i < 3; i++) {
        springs[i + 31].mp0_index = 4;
        springs[i + 31].mp1_index = i + 6;
    }

    for (int i = 0; i < 3; i++) {
        springs[i + 34].mp0_index = 5;
        springs[i + 34].mp1_index = i + 7;
    }

    for (int i = 0; i < nSprings; i++) {
        mSpring* spr = &springs[i];
        updateSpring(spr);
        spr->restLen = glm::length(massPoints[spr->mp0_index].x - massPoints[spr->mp1_index].x);
        spr->stiffness = 1000;
    }
    
    std::cout << "Springs loaded" << std::endl;
}

void Scene4::simulateStep()
{
    if (paused) return;
    float realtimeDt = ImGui::GetIO().DeltaTime;

    curDeltaT += realtimeDt;
    for (; curDeltaT >= deltaT; curDeltaT -= deltaT) {
        (this->*Scene4::simFunc)();
    }
}

void Scene4::simulateSceneEuler() {
    for (int i = 0; i < nSprings; i++) {
        computeElasticSpringForce(&springs[i], massPoints);
    }

    for (int i = 0; i < nMasspoints; i++) {
        calculateEulerStep(&massPoints[i], deltaT);
    }
}

void Scene4::simulateSceneMidpoint() {
    for (int i = 0; i < nSprings; i++) {
        computeElasticSpringForce(&springs[i], massPoints);
    }

    for (int i = 0; i < nMasspoints; i++) {
        massPointsMid[i].x = massPoints[i].x;
        massPointsMid[i].v = massPoints[i].v;
        massPointsMid[i].F = massPoints[i].F;
        calculateEulerStep(&massPointsMid[i], deltaT / 2);
    }

    for (int i = 0; i < nSprings; i++) {
        computeElasticSpringForce(&springs[i], massPointsMid);
    }

    for (int i = 0; i < nMasspoints; i++) {
        calculateMidpointStep(&massPoints[i], &massPointsMid[i]);
    }
}

void Scene4::updateSpring(mSpring* spr) {
    spr->F01 = glm::vec3(0); // reset F to be recomputed with new mp positions
}

void Scene4::computeElasticSpringForce(mSpring* spr, mMassPoint* mps) {
    updateSpring(spr);
    glm::vec3 springVec = mps[spr->mp0_index].x - mps[spr->mp1_index].x;
    float sprLen = glm::length(springVec);
    spr->F01 = ((-spr->stiffness * global_stiffness_multiplier / sprLen) * (sprLen - spr->restLen)) * springVec;

    mps[spr->mp0_index].F += spr->F01;
    mps[spr->mp1_index].F -= spr->F01;
}

void Scene4::calculateEulerStep(mMassPoint* mp, float h) {
    glm::vec3 F = mp->F + extF + mp->mass * g;

    mp->v = mp->v + h * (F/mp->mass);

    glm::vec3 newPos = mp->x + h * mp->v;
    if (newPos.z <= floor || newPos.z >= ceiling) {
        mp->v *= glm::vec3(1, 1, -1);
        newPos = mp->x + h * mp->v;
    }
    mp->x = newPos;

    mp->F = glm::vec3(0);
}

void Scene4::calculateMidpointStep(mMassPoint* mp, mMassPoint* mpMid) {
    glm::vec3 F = mpMid->F + extF + mp->mass * g;

    mp->v = mp->v + deltaT * (F / mp->mass);
    
    mp->x = mp->x + deltaT * mpMid->v;

    glm::vec3 newPos = mp->x + deltaT * mp->v;
    if (newPos.z <= floor || newPos.z >= ceiling) {
        mp->v *= glm::vec3(1, 1, -1);
        newPos = mp->x + deltaT * mp->v;
    }
    mp->x = newPos;

    mp->F = glm::vec3(0);
}
