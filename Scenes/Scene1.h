#include <random>

#include "Scene.h"

struct Particle
{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float lifetime;
};

class Scene1 : public Scene
{
    float pitch = 0.f;
    float roll = 0.f;
    float yaw = 0.f;

    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;

    float pitch_increment = 0.005f;
    float roll_increment = 0.005f;
    float yaw_increment = 0.005f;

    int32_t launch_delay = 8;
    int32_t lastLaunch = 0;
    void launchSphere();

    virtual void onGUI() override;

    float particle_lifetime = 1.5f;

    std::vector<Particle> particles;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    Colormap cmap = Colormap("inferno");

    public:
        Scene1() : gen(rd()), dis(0.f, 2.f) {}
};

