#include "Scene.h"
#include <imgui.h>
#include <random>

struct Particle
{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float lifetime;
};

class Scene01 : public Scene
{
    float pitch = 0.f;
    float roll = 0.f;
    float yaw = 0.f;

    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;

    float pitch_increment = 0.001f;
    float roll_increment = 0.002f;
    float yaw_increment = 0.003f;
    virtual void onGUI() override;

    std::vector<Particle> particles;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;
    
    int32_t launch_delay = 8;
    int32_t lastLaunch = 0;
    void launchSphere();

    public:
        Scene01() : gen(rd()), dis(0.f, 1.f) {}
};
