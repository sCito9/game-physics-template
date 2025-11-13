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
    float damping = 1.0f;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis4;

    //fun stuff
    glm::vec3 gravity;
    glm::float32 bounce_Factor = -0.4f;
    //perfect bounce, to add damping go -1.f < x < 0.1f, to make it bounce more go < -1.f
    float time_step = 0.005f;
    float cur_delta_time = 0.f;

    //AABB
    glm::vec3 cube_min;
    glm::vec3 cube_max;

    //Points and springs
    Point points[10] = {};
    Spring springs[37] = {};

    //UI stuff
    enum class State { Euler_Simulation, Midpoint_Simulation };

    State currentState = State::Midpoint_Simulation;

public:
    Complex_Simulation() : gen(rd()), dis4(0.5f, 1.5f)
    {
        gravity = glm::vec3(0.f, 0.f, -9.81f);

        //AABB
        cube_min = glm::vec3(-2.5f, -2.5f, -2.5f);
        cube_max = glm::vec3(2.5f, 2.5f, 2.5f);


        points[0] = {
            glm::vec3(0.f, 0.f, -2.f), glm::vec3(0.f, 0.f, 0.f), 10.f,
            glm::vec4(dis4(gen), dis4(gen), dis4(gen), 1.0f), false
        };
        points[1] = {
            glm::vec3(-1.f, -1.f, -1.f), glm::vec3(0.f, 0.f, 0.f), 10.f,
            glm::vec4(dis4(gen), dis4(gen), dis4(gen), 1.0f), false
        };
        points[2] = {
            glm::vec3(1.f, -1.f, -1.f), glm::vec3(0.f, 0.f, 0.f), 10.f,
            glm::vec4(dis4(gen), dis4(gen), dis4(gen), 1.0f), false
        };
        points[3] = {
            glm::vec3(1.f, 1.f, -1.f), glm::vec3(0.f, 0.f, 0.f), 10.f,
            glm::vec4(dis4(gen), dis4(gen), dis4(gen), 1.0f), false
        };
        points[4] = {
            glm::vec3(-1.f, 1.f, -1.f), glm::vec3(0.f, 0.f, 0.f), 10.f,
            glm::vec4(dis4(gen), dis4(gen), dis4(gen), 1.0f), false
        };
        points[5] = {
            glm::vec3(-1.f, -1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), 10.f,
            glm::vec4(dis4(gen), dis4(gen), dis4(gen), 1.0f), false
        };
        points[6] = {
            glm::vec3(1.f, -1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), 10.f,
            glm::vec4(dis4(gen), dis4(gen), dis4(gen), 1.0f), false
        };
        points[7] = {
            glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), 10.f,
            glm::vec4(dis4(gen), dis4(gen), dis4(gen), 1.0f), false
        };
        points[8] = {
            glm::vec3(-1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), 10.f,
            glm::vec4(dis4(gen), dis4(gen), dis4(gen), 1.0f), false
        };
        points[9] = {
            glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f, 0.f, 0.f), 10.f,
            glm::vec4(dis4(gen), dis4(gen), dis4(gen), 1.0f), false
        };

        /*
         *
         *  Springs
         *
         */

        springs[0] = {0, 1, 6000.f, 1.5f};
        springs[1] = {1, 2, 6000.f, 2.f};
        springs[2] = {2, 3, 6000.f, 2.f};
        springs[3] = {3, 4, 6000.f, 2.f};
        springs[4] = {4, 5, 6000.f, 2.f};
        springs[5] = {5, 6, 6000.f, 2.f};
        springs[6] = {6, 7, 6000.f, 2.f};
        springs[7] = {7, 8, 6000.f, 2.f};
        springs[8] = {8, 9, 6000.f, 2.f};
        springs[9] = {9, 0, 6000.f, 5.f};

        //point 0 rest
        springs[10] = {0, 2, 6000.f, 1.5f};
        springs[11] = {0, 3, 6000.f, 1.5f};
        springs[12] = {0, 4, 6000.f, 1.5f};

        //point 9 rest
        springs[13] = {9, 5, 6000.f, 1.5f};
        springs[14] = {9, 6, 6000.f, 1.5f};
        springs[15] = {9, 7, 6000.f, 1.5f};


        //point 1 rest
        springs[16] = {1, 3, 6000.f, 2.f};
        springs[17] = {1, 4, 6000.f, 2.f};
        springs[18] = {1, 5, 6000.f, 2.f};
        springs[19] = {1, 6, 6000.f, 2.f};
        springs[20] = {1, 7, 6000.f, 3.f};
        springs[21] = {1, 8, 6000.f, 2.f};

        //point 2 rest
        springs[22] = {2, 4, 6000.f, 2.f};
        springs[23] = {2, 5, 6000.f, 2.f};
        springs[24] = {2, 6, 6000.f, 2.f};
        springs[25] = {2, 7, 6000.f, 2.f};
        springs[26] = {2, 8, 6000.f, 3.f};

        //point 3 rest
        springs[27] = {3, 5, 6000.f, 3.f};
        springs[28] = {3, 6, 6000.f, 2.f};
        springs[29] = {3, 7, 6000.f, 2.f};
        springs[30] = {3, 8, 6000.f, 2.f};

        //point 4 rest
        springs[31] = {4, 6, 6000.f, 3.f};
        springs[32] = {4, 7, 6000.f, 2.f};
        springs[33] = {4, 8, 6000.f, 2.f};

        //point 5 rest
        springs[34] = {5, 7, 6000.f, 2.f};
        springs[35] = {5, 8, 6000.f, 2.f};
        springs[36] = {5, 9, 6000.f, 1.5f};


        for (auto& spring : springs)
        {
            glm::vec3 p1 = points[spring.point_1].position;
            glm::vec3 p2 = points[spring.point_2].position;
            spring.initial_Length = glm::length(p1 - p2);
        }
    }
};
