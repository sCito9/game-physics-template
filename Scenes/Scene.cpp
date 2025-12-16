#include "Scene.h"
#include <imgui.h>
#include "util/CollisionDetection.h"

void Scene::onDraw(Renderer &renderer)
{
    getTemperatureFieldToDraw();
    renderer.drawImage(drawnTemperatureValues, m+2, n+2);

    cameraMatrix = renderer.camera.viewMatrix;

    camPos = renderer.camera.position;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);

    //debug:
}

void Scene::getTemperatureFieldToDraw() {
    drawnTemperatureValues.clear();
    for (const auto &row : temperatureField) {
        for (float element : row) {
            drawnTemperatureValues.push_back(element);
        }
    }
}


