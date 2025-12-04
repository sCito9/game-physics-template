#include "Scene.h"
#include <map>

#include "Scenes/Ex2/Scene_1.h"
#include "Scenes/Ex2/Scene_2.h"
#include "Scenes/Ex2/Scene_3.h"

using SceneCreator = std::function<std::unique_ptr<Scene>()>;

template <typename T>
SceneCreator creator()
{
    return []()
    { return std::make_unique<T>(); };
}

std::map<std::string, SceneCreator> scenesCreators = {
    //{"Demo Scene", creator<Scene01>()},
    //{"Demo Scene 2", creator<Scene02>()},
        // add more Scene types here
        /*
    {"Single Step", creator<Scene1>()},
    {"Euler Simulation", creator<Scene2>()},
    {"Midpoint Simulation", creator<Scene3>()},
    {"Complex Simulation", creator<Scene4>()},
         */
    {"Single Step", creator<Scene_1>()},
    {"Simulation", creator<Scene_2>()},
    {"Collision", creator<Scene_3>()},
};
