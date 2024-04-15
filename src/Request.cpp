#include "Request.hpp"

void Request::setCP(int cp) {
	Request::m_cp = cp;
}

void Request::performCPRequest() {

	if (Request::m_openGameChecked) {
		return;
	}

    int accountID = GJAccountManager::get()->m_accountID;
    int totalEXP = 0;
    web::AsyncWebRequest()
        .postRequest()
        .bodyRaw(fmt::format("targetAccountID={}&secret={}", accountID, "Wmfd2893gb7"))
        .fetch("http://www.boomlings.com/database/getGJUserInfo20.php")
        .text()
        .then([totalEXP](const std::string& response) {
            
            Request::m_openGameChecked = true;
            Request::m_cp = std::stoi(parseRequest(response, "8"));

            int originalEXP = Mod::get()->getSavedValue<int>("total-exp");
            generateNewTotalEXP();

            if (Mod::get()->getSettingValue<bool>("disable-open-check")) {
                return;
            }

			int newEXP = Request::currentTotalEXP();
			int currentLevel = LevelHelper::getLevelFromEXP(originalEXP);
			int nextLevel = LevelHelper::getLevelFromEXP(newEXP);
			int nextLevelEXP = LevelHelper::getEXPRequiredForLevel(nextLevel);

			auto scene = CCDirector::sharedDirector()->getRunningScene();

                Loader::get()->queueInMainThread([scene, originalEXP, newEXP, currentLevel, nextLevelEXP, nextLevel] {

                    CCLayer* parentLayer = nullptr;
                    CCObject* obj;
                    CCARRAY_FOREACH(scene->getChildren(), obj) {
                        auto ccl = typeinfo_cast<CCLayer*>(obj);
                        if (ccl != nullptr) {
                            parentLayer = ccl;
                            break;
                        }
                    }

                    if (nextLevel > currentLevel) {
                        TierBarPopup::createPopupSubroutine(scene, nextLevelEXP, newEXP, 4.5f);
                        if (parentLayer != nullptr) {
                            auto popup = LevelUpPopup::create(currentLevel, nextLevel);
                            popup->m_scene = parentLayer;
                            popup->show();
                        }   
                    } else {
                        TierBarPopup::createPopupSubroutine(scene, originalEXP, newEXP, 0);
                    }
                
                
            });
            
        });
}

int Request::generateNewTotalEXP() {
	
	auto stats = GameStatsManager::sharedState()->m_playerStats;
	Request::m_stars = std::stoi((stats->valueForKey("6"))->m_sString);
	Request::m_moons = std::stoi((stats->valueForKey("28"))->m_sString);
	Request::m_diamonds = std::stoi((stats->valueForKey("13"))->m_sString);
	Request::m_gold_coins = std::stoi((stats->valueForKey("8"))->m_sString);
	Request::m_user_coins = std::stoi((stats->valueForKey("12"))->m_sString);
	Request::m_demons = std::stoi((stats->valueForKey("5"))->m_sString);

	int totalEXP = currentTotalEXP();

	Mod::get()->setSavedValue<int>("total-exp", totalEXP);

	return totalEXP;
}

int Request::currentTotalEXP() {
	int totalEXP = 0;

	totalEXP += Request::m_stars * STAR_EXP;
	totalEXP += Request::m_moons * MOON_EXP;
	totalEXP += Request::m_diamonds * DIAMOND_EXP;
	totalEXP += Request::m_gold_coins * GOLD_COIN_EXP;
	totalEXP += Request::m_user_coins * USER_COIN_EXP;
	totalEXP += Request::m_demons * DEMON_EXP;
	totalEXP += Request::m_cp * CP_EXP;

	Request::m_currentTotalEXP = totalEXP;

	return totalEXP;
}

void Request::displayTotalEXP() {
	auto notif = Notification::create(std::to_string(currentTotalEXP()).c_str(), CCSprite::createWithSpriteFrameName("GJ_sMagicIcon_001.png"), 1.5f);
	notif->show();
}

std::string Request::parseRequest(const std::string& object, const std::string& key) {
	std::unordered_map<std::string, std::string> keyValueMap;

	// Parse the object string
	std::istringstream iss(object);
	std::string token;
	while (std::getline(iss, token, ':')) {
		std::string key = token;
		if (std::getline(iss, token, ':')) {
			std::string value = token;
			keyValueMap[key] = value;
		}
	}

	// Find and return the value corresponding to the given key
	auto it = keyValueMap.find(key);
	if (it != keyValueMap.end()) {
		return it->second;
	} else {
		return ""; // Key not found
	}
}