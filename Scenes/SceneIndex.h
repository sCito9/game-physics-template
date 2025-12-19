#include "Scene.h"
#include <map>

#include "Scenes/Ex3/Scene_01.h"
#include "Scenes/Ex3/Scene_02.h"
#include "Scenes/Ex3/Scene_03.h"
#include "Scenes/Ex3/Scene_04.h"

using SceneCreator = std::function<std::unique_ptr<Scene>()>;

template <typename T>
SceneCreator creator()
{
    return []()
    { return std::make_unique<T>(); };
}

std::map<std::string, SceneCreator> scenesCreators = {
    {"Single Step", creator<Scene_01>()},
    {"Explicit Simulation", creator<Scene_02>()},
    {"Implicit Simulation", creator<Scene_03>()},
    {"Interactive Simulation", creator<Scene_04>()},
};
