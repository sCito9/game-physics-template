#include "Scene.h"
#include <map>

#include "PDE_Explicit_Simulation.h"
#include "PDE_Single_Step.h"


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
    {"Single Step", creator<PDE_Single_Step>()},
    {"Explicit Simulation", creator<PDE_Explicit_Simulation>()}

    // add more Scene types here
};
