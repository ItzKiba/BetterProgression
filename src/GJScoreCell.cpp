#include <Geode/modify/GJScoreCell.hpp>
#include "SpriteHelper.hpp"
#include "LevelHelper.hpp"

class $modify(BP_GJScoreCell, GJScoreCell) {
	void loadFromScore(GJUserScore* p0) {
		GJScoreCell::loadFromScore(p0);

		// get main layer
		CCLayer* mainLayer = nullptr;
		CCObject* obj;
		CCARRAY_FOREACH(this->getChildren(), obj) {
			CCLayer* search = typeinfo_cast<CCLayer*>(obj);
			if (search != nullptr && search->getChildrenCount() > 2) {
				mainLayer = search;
				break;
			}
		}

		int currentEXP = LevelHelper::calculateTotalEXP(p0);
		int currentLevel = LevelHelper::getLevelFromEXP(currentEXP);

		auto badgeSpr = SpriteHelper::createFullSprite(currentLevel);
		badgeSpr->setPosition({75, 17.5});
		badgeSpr->setScale(0.62);

		mainLayer->addChild(badgeSpr);

		auto statsMenu = mainLayer->getChildByIDRecursive("stats-menu");
		statsMenu->setScale(0.83);
		statsMenu->setPositionX(statsMenu->getPositionX() + 13.f);
	}
};