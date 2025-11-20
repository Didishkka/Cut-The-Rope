#include "BaseScene.h"
#include <iostream>

BaseScene::BaseScene()
{
    oldSceneId = 0;
    newSceneId = 0;

    //связываем ключи-значения для сцен
    mapping["start"] = 0;
    mapping["loading"] = 1;
    mapping["level_map"] = 2;
    mapping["level_one"] = 3;
    mapping["level_two"] = 4;
    mapping["level_three"] = 5;
    mapping["level"] = 6;
    mapping["settings"] = 7;
}