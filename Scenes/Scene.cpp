#include "Scene.h"
#include <imgui.h>

void Scene::onDraw(Renderer& renderer)
{
    cameraMatrix = renderer.camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);

    //personal values
    width = renderer.camera.width;
    height = renderer.camera.height;
    projectionMatrix = renderer.camera.projectionMatrix();
    camera_Position = renderer.camera.position;
}
