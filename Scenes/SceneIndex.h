#include "Scene.h"
#include <map>

#include "Collision.h"
#include "Complex_Simulation_Rb.h"
#include "PDE_Explicit_Simulation.h"
#include "PDE_Implicit_Simulation.h"
#include "PDE_Single_Step.h"
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
    /*{"Single Step", creator<PDE_Single_Step>()},
    {"Explicit Simulation", creator<PDE_Explicit_Simulation>()},
    {"Implicit Simulation", creator<PDE_Implicit_Simulation>()}*/
    {"Single Step", creator<Single_Step>()},
    {"Simulation", creator<Simulation>()},
    {"Collision", creator<Collision>()},
    {"Complex Simulation", creator<Complex_Simulation_Rb>()}

    // add more Scene types here
};
