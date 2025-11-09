//
// Created by tonib on 09.11.2025.
//

#include "Scene4.h"
#include <imgui.h>

void Scene4::init() {
    MassPoint* point1 = new MassPoint(10, {0, 0, -2}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0});
    MassPoint* point2 = new MassPoint(10, {-1, -1, -1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0});
    MassPoint* point3 = new MassPoint(10, {1, -1, -1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0});
    MassPoint* point4 = new MassPoint(10, {1, 1, -1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0});
    MassPoint* point5 = new MassPoint(10, {-1, 1, -1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0});
    MassPoint* point6 = new MassPoint(10, {-1, -1, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0});
    MassPoint* point7 = new MassPoint(10, {1, -1, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0});
    MassPoint* point8 = new MassPoint(10, {1, 1, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0});
    MassPoint* point9 = new MassPoint(10, {-1, 1, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0});
    MassPoint* point10 = new MassPoint(10, {0, 0, 2}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0});
    massPoints.push_back(point1);
    massPoints.push_back(point2);
    massPoints.push_back(point3);
    massPoints.push_back(point4);
    massPoints.push_back(point5);
    massPoints.push_back(point6);
    massPoints.push_back(point7);
    massPoints.push_back(point8);
    massPoints.push_back(point9);
    massPoints.push_back(point10);

    Spring* spring1 = new Spring(10000, 1.5, point1, point2);
    Spring* spring2 = new Spring(10000, 1.5, point1, point3);
    Spring* spring3 = new Spring(10000, 1.5, point1, point4);
    Spring* spring4 = new Spring(10000, 1.5, point1, point5);
    Spring* spring5 = new Spring(10000, 2, point2, point3);
    Spring* spring6 = new Spring(10000, 2, point3, point4);
    Spring* spring7 = new Spring(10000, 2, point4, point5);
    Spring* spring8 = new Spring(10000, 2, point5, point2);
    Spring* spring9 = new Spring(10000, 2, point2, point6);
    Spring* spring10 = new Spring(10000, 2, point2, point7);
    Spring* spring11 = new Spring(10000, 2, point2, point9);
    Spring* spring12 = new Spring(10000, 2, point3, point6);
    Spring* spring13 = new Spring(10000, 2, point3, point7);
    Spring* spring14 = new Spring(10000, 2, point3, point8);
    Spring* spring15 = new Spring(10000, 2, point4, point7);
    Spring* spring16 = new Spring(10000, 2, point4, point8);
    Spring* spring17 = new Spring(10000, 2, point4, point9);
    Spring* spring18 = new Spring(10000, 2, point5, point8);
    Spring* spring19 = new Spring(10000, 2, point5, point9);
    Spring* spring20 = new Spring(10000, 2, point5, point6);
    Spring* spring21 = new Spring(10000, 2, point6, point7);
    Spring* spring22 = new Spring(10000, 2, point7, point8);
    Spring* spring23 = new Spring(10000, 2, point8, point9);
    Spring* spring24 = new Spring(10000, 2, point9, point6);
    Spring* spring25 = new Spring(10000, 1.5, point10, point6);
    Spring* spring26 = new Spring(10000, 1.5, point10, point7);
    Spring* spring27 = new Spring(10000, 1.5, point10, point8);
    Spring* spring28 = new Spring(10000, 1.5, point10, point9);
    Spring* spring29 = new Spring(10000, 2, point2, point4);
    Spring* spring30 = new Spring(10000, 2, point3, point5);
    Spring* spring31 = new Spring(10000, 2, point6, point8);
    Spring* spring32 = new Spring(10000, 2, point7, point9);
    Spring* spring33 = new Spring(10000, 5, point1, point10);
    Spring* spring34 = new Spring(10000, 3, point3, point9);
    Spring* spring35 = new Spring(10000, 3, point4, point6);
    Spring* spring36 = new Spring(10000, 3, point5, point7);
    Spring* spring37 = new Spring(10000, 3, point2, point8);
    springs.push_back(spring1);
    springs.push_back(spring2);
    springs.push_back(spring3);
    springs.push_back(spring4);
    springs.push_back(spring5);
    springs.push_back(spring6);
    springs.push_back(spring7);
    springs.push_back(spring8);
    springs.push_back(spring9);
    springs.push_back(spring10);
    springs.push_back(spring11);
    springs.push_back(spring12);
    springs.push_back(spring13);
    springs.push_back(spring14);
    springs.push_back(spring15);
    springs.push_back(spring16);
    springs.push_back(spring17);
    springs.push_back(spring18);
    springs.push_back(spring19);
    springs.push_back(spring20);
    springs.push_back(spring21);
    springs.push_back(spring22);
    springs.push_back(spring23);
    springs.push_back(spring24);
    springs.push_back(spring25);
    springs.push_back(spring26);
    springs.push_back(spring27);
    springs.push_back(spring28);
    springs.push_back(spring29);
    springs.push_back(spring30);
    springs.push_back(spring31);
    springs.push_back(spring32);
    springs.push_back(spring33);
    springs.push_back(spring34);
    springs.push_back(spring35);
    springs.push_back(spring36);
    springs.push_back(spring37);
}
bool shouldSimulate = true;
void Scene4::onGUI() {
    ImGui::InputFloat("Timestep: ", &timeStep);
    ShowDropdownMenuForMethods();
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        shouldSimulate = !shouldSimulate;
    }
}

static const char* methods[] = { "Euler Method", "Midpoint Method" }; // Options for the dropdown
static int currentMethod = 1; // Index of the currently selected item
void Scene4::ShowDropdownMenuForMethods() {

    // Create the dropdown menu
    if (ImGui::BeginCombo("Choose a Method", methods[currentMethod])) {
        for (int i = 0; i < IM_ARRAYSIZE(methods); i++) {
            bool isSelected = (currentMethod == i); // Check if this item is selected
            if (ImGui::Selectable(methods[i], isSelected)) {
                currentMethod = i; // Update the selected item
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus(); // Focus the selected item
            }
        }
        ImGui::EndCombo(); // End the dropdown menu
    }
}

void Scene4::simulateStep() {
    if (!shouldSimulate) {  //possibility to stop simulation
        return;
    }

    glm::vec3 gravity = {0, 0, -9.81f};

    for (MassPoint* point : massPoints) {
        point->acceleration += gravity;
    }

    switch (currentMethod) {
        case 0:
            calculateEulerStep();
            break;
        case 1:
            calculateMidpointStep();
            break;
        default:
            break;
    }

    //box boundaries
    for (MassPoint* point : massPoints) {
        if (point->position.x > 2.5f || point->position.x < -2.5f || point->position.y > 2.5f || point->position.y < -2.5f || point->position.z > 2.5f || point->position.z < -2.5f) {
            point->velocity *= 0.5;    //reibung

            if (point->position.x > 2.5f) {
                point->position.x = 2.5f;
            }
            else if (point->position.x < -2.5f) {
                point->position.x = -2.5f;
            }

            if (point->position.y > 2.5f) {
                point->position.y = 2.5f;
            }
            else if (point->position.y < -2.5f) {
                point->position.y = -2.5f;
            }

            if (point->position.z > 2.5f) {
                point->position.z = 2.5f;
            }
            else if (point->position.z < -2.5f) {
                point->position.z = -2.5f;
            }
        }
    }

    //mouse interaction
    if(ImGui::IsMouseReleased(ImGuiMouseButton_Right)){
        auto drag = ImGui::GetMouseDragDelta(1);
        if(!(drag.x == 0 && drag.y == 0)){
            auto dx = drag.x * right;
            auto dy = -drag.y * up;
            for (MassPoint* point : massPoints) {
                point->velocity += (dx + dy) * 0.01f;
            }
        }
    }
}

void Scene4::calculateMidpointStep() {  //with added gravity
    glm::vec3 gravity = {0, 0, -9.81f};

    for (MassPoint* point : massPoints) {
        glm::vec3 midpointPos = point->position + timeStep/2 * point->velocity;

        point->resetForces();
        computeCurrentForcesOnPoint(point);
        point->acceleration = point->force / point->mass + gravity;

        glm::vec3 midpointVel = point->velocity + timeStep/2 * point->acceleration;

        glm::vec3 tmp = point->position + timeStep/2 * midpointVel;
        point->position = midpointPos;  //xtmp benutzen um neue forces zu berechnen

        point->resetForces();   //a(t+h/2)
        computeCurrentForcesOnPoint(point);
        point->acceleration = point->force / point->mass + gravity;

        point->position = tmp;
        point->velocity += timeStep * point->acceleration;
    }
}
