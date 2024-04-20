#include <Geode/modify/MenuLayer.hpp>
#include "LevelHelper.hpp"
#include "SpriteHelper.hpp"
#include "Request.hpp"
#include "Manager.hpp"
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
		Manager::getSharedInstance()->isWherwin = (GameManager::get()->m_playerUserID.value() == 3864233);
		return true;
	}

};