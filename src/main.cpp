#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <Geode/modify/ChallengesPage.hpp>
#include <Geode/modify/DailyLevelNode.hpp>

#include "LevelUpPopup.hpp"

#include "Request.hpp"
#include "TierBarPopup.hpp"

bool quitFromPlayLayer = false;

class $modify(BP_PlayLayer, PlayLayer) {

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }

        Request::generateNewTotalEXP();
        //Request::displayTotalEXP();


        return true;
    }

    void onQuit() {
        PlayLayer::onQuit();
        quitFromPlayLayer = true;
    }
};

class $modify(BP_CCDirector, CCDirector) {
    void willSwitchToScene(CCScene* scene) {
        int originalEXP = Request::currentTotalEXP();

        CCDirector::willSwitchToScene(scene);

        Request::generateNewTotalEXP();

        if (!quitFromPlayLayer) {
            return;
        }

        quitFromPlayLayer = false;

        int newEXP = Request::currentTotalEXP();
        int currentLevel = LevelHelper::getLevelFromEXP(originalEXP);
        int nextLevel = LevelHelper::getLevelFromEXP(newEXP);
        int nextLevelEXP = LevelHelper::getEXPRequiredForLevel(nextLevel);

        Loader::get()->queueInMainThread([this, scene, originalEXP, newEXP, currentLevel, nextLevelEXP, nextLevel] {
            
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
            } else if (!(Mod::get()->getSettingValue<bool>("disable-tier-bar"))) {
                TierBarPopup::createPopupSubroutine(scene, originalEXP, newEXP, 0);
            }
        });
    }
};