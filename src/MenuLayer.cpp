#include <Geode/modify/MenuLayer.hpp>
#include "LevelHelper.hpp"
#include "SpriteHelper.hpp"
#include "Request.hpp"
#include <Geode/loader/Loader.hpp>
#include <string>

using namespace geode::prelude;

class $modify(BP_MenuLayer, MenuLayer) {

    struct Fields {
        bool m_badgeExists = false;
    };

    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }
        Request::setupListener();
        Request::performCPRequest();
        
        return true;
    }

};