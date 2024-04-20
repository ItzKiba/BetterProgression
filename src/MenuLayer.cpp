#include <Geode/modify/MenuLayer.hpp>
#include "LevelHelper.hpp"
#include "SpriteHelper.hpp"
#include "Request.hpp"
#include <Geode/loader/Loader.hpp>
#include <string>

using namespace geode::prelude;

class $modify(BP_MenuLayer, MenuLayer) {
    bool m_badgeExists = false;

    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }
        Request::performCPRequest();
        
        return true;
    }

};