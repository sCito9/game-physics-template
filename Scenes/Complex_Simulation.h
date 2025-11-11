//
// Created by felix on 31.10.2025.
//
#include <random>

#include "Scene.h"


class Complex_Simulation : public Scene
{
    //methods to overwrite
    virtual void onDraw(Renderer& renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;

    //own methods
    void checkCollision();
    void midpointMethod(double len, glm::dvec3 diff, glm::dvec3 F01, glm::dvec3 F10);
    void eulerMethod(double len, glm::dvec3 diff, glm::dvec3 F01, glm::dvec3 F10);

    //random variables
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;
    std::uniform_real_distribution<float> dis2;
    std::uniform_real_distribution<float> dis3;
    std::uniform_real_distribution<float> dis4;

    //fun stuff
    glm::vec3 gravity;
    glm::float32 bounce_Factor = -1.f;
    //perfect bounce, to add damping go -1.f < x < 0.1f, to make it bounce more go < -1.f
    float time_step = 0.005f;
    float cur_delta_time = 0.f;

    //AABB
    glm::vec3 cube_min;
    glm::vec3 cube_max;

    //Points and springs
    Point points[10] = {};
    Spring springs[14] = {};

    //UI stuff
    enum class State { Euler_Simulation, Midpoint_Simulation };

    State currentState = State::Euler_Simulation;

public:
    Complex_Simulation() : gen(rd()), dis(-1.5f, 1.5f), dis2(5.f, 20.f), dis3(35.f, 45.f), dis4(0.5f, 1.5f)
    {
        gravity = glm::vec3(0.f, 0.f, -9.81f);

        //AABB
        cube_min = glm::vec3(-2.5f, -2.5f, -2.5f);
        cube_max = glm::vec3(2.5f, 2.5f, 2.5f);


        for (int i = 0; i < 10; i++)
        {
            points[i] = {
                glm::vec3(dis(gen), dis(gen), dis(gen)), glm::vec3(dis(gen), dis(gen), dis(gen)), dis2(gen),
                glm::vec4(dis4(gen), dis4(gen), dis4(gen), 1.0f), false
            };
        }


        for (int i = 0; i < 10; i++)
        {
            springs[i] = {i, (i + 1) % 10, dis3(gen), dis4(gen)};
        }
        springs[10] = {0, 8, dis3(gen), dis4(gen)};
        springs[11] = {2, 6, dis3(gen), dis4(gen)};
        springs[12] = {4, 8, dis3(gen), dis4(gen)};
        springs[13] = {7, 9, dis3(gen), dis4(gen)};
    }
};
