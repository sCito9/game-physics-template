#pragma once
#include <list>

#include "Renderer.h"

/// @brief Scene base class. **Run `cmake . -B build` after adding new files to the scenes folder**
///
/// This class is the base class for all scenes in the application.
/// When creating a new scene, derive from this class.
/// Add the new scene to the SceneIndex.h file.
///
/// Feel free to modify the non virtual functions for functionality shared between scenes.
class Scene
{
public:
    /// @brief Initialize the scene. Gets called every time the scene is switched to.
    virtual void init() {};
    /// @brief Simulate a step in the scene. Gets called every frame before onDraw.
    ///
    /// This is where you should update the physics of the scene.
    virtual void simulateStep() {};
    /// @brief Draw the scene. Gets called every frame after simulateStep.
    ///
    /// This is where you should call the Renderer draw functions.
    virtual void onDraw(Renderer &renderer);
    /// @brief Define the GUI for the scene. Gets called every frame after onDraw.
    virtual void onGUI() {};
    virtual ~Scene() = default;

    std::vector<std::vector<float>> temperatureField = {};
    std::vector<std::vector<float>> domain = {}; //spatial dimensions (y extent) X (x extent)
    int m = 0;    //interior dimension
    int n = 0;    //interior dimension
    float delta_x = 0;
    float delta_y = 0;
    float timeStep = 0.01f;
    float thermalDiffusivity = 0.1f;

    float currentTime = 0;
    bool shouldSimulate = false;
    bool realDeltaTime = true;

    std::vector<float> drawnTemperatureValues {};

    glm::mat4 cameraMatrix = glm::mat4(1);
    glm::vec3 camPos = glm::vec3(0);
    glm::vec3 fwd = glm::vec3(1, 0, 0);
    glm::vec3 right = glm::vec3(0, 1, 0);
    glm::vec3 up = glm::vec3(0, 0, 1);
    int windowHeight;
    int windowWidth;

    void getTemperatureFieldToDraw();
};

inline std::ostream& operator << (std::ostream& os, const glm::vec3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}
