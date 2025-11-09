#include "Scene.h"
#include <imgui.h>

void Scene::onDraw(Renderer &renderer)
{
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));
    for (MassPoint* point : massPoints) {
        renderer.drawSphere(point->position, point->mass/100, {1, 0, 0, 1});
    }
    for (Spring* spring : springs) {
        renderer.drawLine(spring->point1->position, spring->point2->position, {0, 0, 1});
    }
}

void Scene::calculateEulerStep() {
    for (MassPoint* point : massPoints) {
        point->position += timeStep * point->velocity;

        point->velocity += timeStep * point->acceleration;

        point->acceleration = point->force / point->mass;

        point->resetForces();   //reset force, damit wieder alle springs eine neue für diesen step berechnen können
    }
    for (Spring* spring : springs) {
        glm::vec3 force1_2 = -1 * (spring->stiffness) * (spring->getCurrentLength() - spring->restLength) * (1/spring->getCurrentLength() * (spring->point1->position - spring->point2->position));
        spring->point1->force += force1_2;
        glm::vec3 force2_1 = -force1_2;
        spring->point2->force += force2_1;
    }
}

void Scene::calculateMidpointStep() {
    for (MassPoint* point : massPoints) {
        glm::vec3 midpointPos = point->position + timeStep/2 * point->velocity;

        point->resetForces();
        computeCurrentForcesOnPoint(point);
        point->acceleration = point->force / point->mass;

        glm::vec3 midpointVel = point->velocity + timeStep/2 * point->acceleration;

        glm::vec3 tmp = point->position + timeStep/2 * midpointVel;
        point->position = midpointPos;  //xtmp benutzen um neue forces zu berechnen

        point->resetForces();   //a(t+h/2)
        computeCurrentForcesOnPoint(point);
        point->acceleration = point->force / point->mass;

        point->position = tmp;
        point->velocity += timeStep * point->acceleration;
    }
}

void Scene::computeCurrentForcesOnPoint(MassPoint *point) {
    for (Spring* spring : springs) {
        if (spring->point1 == point) {
            glm::vec3 force1_2 = -1 * (spring->stiffness) * (spring->getCurrentLength() - spring->restLength) * (1/spring->getCurrentLength() * (spring->point1->position - spring->point2->position));
            spring->point1->force += force1_2;
        }
        else if (spring->point2 == point) {
            glm::vec3 force2_1 = -1 * (spring->stiffness) * (spring->getCurrentLength() - spring->restLength) * (1/spring->getCurrentLength() * (spring->point2->position - spring->point1->position));
            spring->point2->force += force2_1;
        }
    }
}
