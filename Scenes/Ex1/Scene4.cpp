#include "Scene4.h"

void Scene4::onDraw(Renderer& renderer){
    cameraMatrix = Renderer::camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);

    renderer.drawQuad(
        glm::vec3(0, 0, floor),
        glm::quat(glm::vec3(0, 0, 0)),
        glm::vec2(floor * 2),
        glm::vec4(.68f, .85f, .9f, 1));

    if (isCubeActive) renderer.drawWireCube(glm::vec3(0),
                                            glm::vec3(floor * 2),
                                            glm::vec3(1));

    for (int i = 0; i < nMasspoints; i++) {
        renderer.drawSphere(
            massPoints[i].x,
            0.1f,
            glm::vec4(1.f - ((float)i)/nMasspoints, 0,((float)i)/nMasspoints, 1));
            //glm::vec4(1, 0, 0, 1));
    }

    auto cmap = Colormap("plasma");
    for (int i = 0; i < nSprings; i++)
    {
        float stauchung = glm::length(massPoints[springs[i].mp0_index].x - massPoints[springs[i].mp1_index].x)
                / springs[i].restLen;
        renderer.drawLine(
                massPoints[springs[i].mp0_index].x,
                massPoints[springs[i].mp1_index].x,
                cmap(stauchung * stauchung));
                // glm::vec4(1, 0.65f, 0, 1));
    }
}

void Scene4::onGUI()
{
    ImGui::LabelText("Info", "[Space] to (un-)pause sim\nSane ver. use semi-euler");
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        paused = !paused;
    }
    if (ImGui::Selectable("Euler", selectedMethod == Euler)) {
        selectedMethod = Euler;
        simFunc = &Scene4::simulateSceneEuler;
    }
    if (ImGui::Selectable("Euler for the sane", selectedMethod == Symplectic)) {
        selectedMethod = Symplectic;
        simFunc = &Scene4::simulateSceneSymplecticEuler;
    }
    if (ImGui::Selectable("Midpoint", selectedMethod == Midpoint)) {
        selectedMethod = Midpoint;
        simFunc = &Scene4::simulateSceneMidpoint;
    }
    if (ImGui::Selectable("Midpoint for the sane", selectedMethod == SymplecticMidpoint)) {
        selectedMethod = SymplecticMidpoint;
        simFunc = &Scene4::simulateSceneSymplecticMidpoint;
    }

    ImGui::InputFloat3("ext Force", (float*) &extF);
    if (ImGui::Selectable("Cube on/off", isCubeActive)) {
        isCubeActive = !isCubeActive;
    }

    ImGui::InputFloat("delta t in s", &deltaT);
    ImGui::InputFloat("mul stiffness", &global_stiffness_multiplier);
    if (ImGui::InputFloat("mass", &mass)) {
        for (int i = 0; i < nMasspoints; i++) {
            (&massPoints[i])->mass = mass;
            (&massPointsMid[i])->mass = mass;
        }
    }
}

void Scene4::init() {
    simFunc = &Scene4::simulateSceneEuler;
    selectedMethod = Euler;

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

    //std::cout << "Masspoints loaded" << std::endl;

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
        spr->stiffness = 40;
    }
    
    //std::cout << "Springs loaded" << std::endl;
}

void Scene4::simulateStep()
{
    if (paused || deltaT <= 0.f || mass == 0.f) return;
    float realtimeDt = ImGui::GetIO().DeltaTime;

    auto mouseDrag = glm::vec3(0);
    if(ImGui::IsMouseReleased(ImGuiMouseButton_Right)){
        auto drag = ImGui::GetMouseDragDelta(1);
        if(!(drag.x == 0 && drag.y == 0)){
            auto dx = drag.x * right;
            auto dy = -drag.y * up;
            mouseDrag = (dx + dy) * realtimeDt;
        }
    }

    for (int i = 0; i < nMasspoints; i++) {
        massPoints[i].v += mouseDrag;
        massPointsMid[i].v += mouseDrag;
    }

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

void Scene4::simulateSceneSymplecticEuler() {
    for (int i = 0; i < nSprings; i++) {
        computeElasticSpringForce(&springs[i], massPoints);
    }

    for (int i = 0; i < nMasspoints; i++) {
        calculateSymplecticEulerStep(&massPoints[i], deltaT);
    }
}

void Scene4::simulateSceneMidpoint() {
    for (int i = 0; i < nSprings; i++) {
        computeElasticSpringForce(&springs[i], massPoints);
    }

    for (int i = 0; i < nMasspoints; i++) {
        massPointsMid[i] = massPoints[i];
        calculateEulerStep(&massPointsMid[i], deltaT / 2);
    }

    for (int i = 0; i < nSprings; i++) {
        computeElasticSpringForce(&springs[i], massPointsMid);
    }

    for (int i = 0; i < nMasspoints; i++) {
        calculateMidpointStep(&massPoints[i], massPointsMid[i].F);
    }
}

void Scene4::simulateSceneSymplecticMidpoint() {
    for (int i = 0; i < nSprings; i++) {
        computeElasticSpringForce(&springs[i], massPoints);
    }

    for (int i = 0; i < nMasspoints; i++) {
        massPointsMid[i] = massPoints[i];
        calculateSymplecticEulerStep(&massPointsMid[i], deltaT / 2);
    }

    for (int i = 0; i < nSprings; i++) {
        computeElasticSpringForce(&springs[i], massPointsMid);
    }

    for (int i = 0; i < nMasspoints; i++) {
        calculateSymplecticMidpointStep(&massPoints[i], massPointsMid[i].F);
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


    glm::vec3 newPos = mp->x + h * mp->v;
    resolveCollision(mp, &newPos);
    mp->x = newPos;

    mp->v = mp->v + h * (F/mp->mass);
    mp->F = glm::vec3(0);
}

void Scene4::calculateSymplecticEulerStep(mMassPoint* mp, float h) {
    glm::vec3 F = mp->F + extF + mp->mass * g;

    mp->v = mp->v + h * (F/mp->mass);

    glm::vec3 newPos = mp->x + h * mp->v;
    resolveCollision(mp, &newPos);
    mp->x = newPos;

    mp->F = glm::vec3(0);
}

void Scene4::calculateMidpointStep(mMassPoint* mp, glm::vec3 midF) {
    glm::vec3 F = midF + extF + mp->mass * g;

    glm::vec3 newPos = mp->x + deltaT * mp->v;
    resolveCollision(mp, &newPos);
    mp->x = newPos;

    mp->v = mp->v + deltaT * (F / mp->mass);
    mp->F = glm::vec3(0);
}

void Scene4::calculateSymplecticMidpointStep(mMassPoint* mp, glm::vec3 midF) {
    glm::vec3 F = midF + extF + mp->mass * g;

    mp->v = mp->v + deltaT * (F / mp->mass);

    glm::vec3 newPos = mp->x + deltaT * mp->v;
    resolveCollision(mp, &newPos);
    mp->x = newPos;

    mp->F = glm::vec3(0);
}

void Scene4::resolveCollision(mMassPoint* mp, glm::vec3* newPos) const {
    if (newPos->z <= floor) {
        mp->v *= glm::vec3(1, 1, -1);
        *newPos = glm::vec3(newPos->x, newPos->y, floor);
        /* auto basePos = glm::vec3(newPos->x, newPos->y, floor);
        float ratioAppr = 1.f - (floor - mp->x.z) / (newPos->z - mp->x.z);
        *newPos = basePos + deltaT * mp->v * ratioAppr; */

    }
    if (!isCubeActive) return;

    if (newPos->x >= -floor) {
        mp->v.x *= -1;
        *newPos = glm::vec3(-floor, newPos->y, newPos->z);
    } else if (newPos->x <= floor) {
        mp->v.x *= -1;
        *newPos = glm::vec3(floor, newPos->y, newPos->z);
    }

    if (newPos->y >= -floor) {
        mp->v.y *= -1;
        *newPos = glm::vec3(newPos->x, -floor, newPos->z);
    } else if (newPos->y <= floor) {
        mp->v.y *= -1;
        *newPos = glm::vec3(newPos->x, floor, newPos->z);
    }

    if (newPos->z >= -floor) {
        mp->v.z *= -1;
        *newPos = glm::vec3(newPos->x, newPos->y, -floor);
    }
}
