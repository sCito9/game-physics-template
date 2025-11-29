#include "Scene.h"
#include <map>

#include "Collision.h"
#include "Complex_Simulation_Rb.h"
#include "Scene1.h"
#include "Simulation.h"
#include "Single_Step.h"

using SceneCreator = std::function<std::unique_ptr<Scene>()>;

template <typename T>
SceneCreator creator()
{
    return []()
    {
        return std::make_unique<T>();
    };
}

std::map<std::string, SceneCreator> scenesCreators = {
    {"Single Step", creator<Single_Step>()},
    {"Simulation", creator<Simulation>()},
    {"Collision", creator<Collision>()},
    {"Complex Simulation", creator<Complex_Simulation_Rb>()}
    // add more Scene types here
};
