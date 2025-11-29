#pragma once
#include "Renderer.h"


/// @brief Scene base class. **Run `cmake . -B build` after adding new files to the scenes folder**
///
/// This class is the base class for all scenes in the application.
/// When creating a new scene, derive from this class.
/// Add the new scene to the SceneIndex.h file.
///
/// Feel free to modify the non virtual functions for functionality shared between scenes.

struct Point
{
    glm::vec3 position;
    glm::vec3 velocity;

    glm::float32 mass;
    glm::vec4 color;
    bool isFixed;
};

struct Spring
{
    //int for index
    int point_1;
    int point_2;
    float stiffness;
    float initial_Length;
};

struct Cube
{
    glm::vec3 position_cm;
    glm::vec3 velocity_cm;
    glm::vec3 points[8];
    glm::vec3 points_world_space[8];
    glm::quat orientation;
    glm::mat3 I_inv;
    glm::mat3 I;
    glm::vec3 L;
    float M;
    glm::vec3 scale;
    bool isFixed;
};


class Scene
{
public:
    /// @brief Initialize the scene. Gets called every time the scene is switched to.
    virtual void init()
    {
    };
    /// @brief Simulate a step in the scene. Gets called every frame before onDraw.
    ///
    /// This is where you should update the physics of the scene.
    virtual void simulateStep()
    {
    };
    /// @brief Draw the scene. Gets called every frame after simulateStep.
    ///
    /// This is where you should call the Renderer draw functions.
    virtual void onDraw(Renderer& renderer);
    /// @brief Define the GUI for the scene. Gets called every frame after onDraw.
    virtual void onGUI()
    {
    };
    virtual ~Scene() = default;

    glm::mat4 cameraMatrix = glm::mat4(1);
    glm::mat4 projectionMatrix = glm::mat4(1);
    glm::vec3 fwd = glm::vec3(1, 0, 0);
    glm::vec3 right = glm::vec3(0, 1, 0);
    glm::vec3 up = glm::vec3(0, 0, 1);
    glm::vec3 camera_Position = glm::vec3(0.f);
    int width = 0;
    int height = 0;
};
