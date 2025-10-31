#include "Scene.h"
#include <map>

#include "Complex_Simulation.h"
#include "Euler_Simulation.h"
#include "Midpoint_Simulation.h"
#include "Scene1.h"
#include "Single_Step.h"

using SceneCreator = std::function<std::unique_ptr<Scene>()>;

template <typename T>
SceneCreator creator()
{
    return []()
    { return std::make_unique<T>(); };
}

std::map<std::string, SceneCreator> scenesCreators = {
    {"Single Step", creator<Single_Step>()},
    {"Euler Simulation", creator<Euler_Simulation>()},
    {"Midpoint Simulation", creator<Midpoint_Simulation>()},
    {"Complex Simulation", creator<Complex_Simulation>()}
    // add more Scene types here
};
