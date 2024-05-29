#include <Geode/modify/ProfilePage.hpp>
#include "LevelHelper.hpp"
#include "SpriteHelper.hpp"
#include "Request.hpp"
#include <Geode/loader/Loader.hpp>
#include <string>

using namespace geode::prelude;

class $modify(BP_ProfilePage, ProfilePage) {

    struct Fields {
        CCLayer* m_mainLayer = nullptr;
        bool m_determinePositions = false;
        bool m_barAdded = false;

        float m_newStatsPositionY;
        float m_newStatsPositionX;
        float m_newPlayerPositionY;
        float m_newIconBGPositionY;
    };

    static void onModify(auto &self) {
        self.setHookPriority("ProfilePage::loadPageFromUserInfo", -1);
    }

    void searchForLayer() {
        if (this->m_fields->m_mainLayer != nullptr) {
            return;
        }
        CCObject* obj;
        CCARRAY_FOREACH(this->getChildren(), obj) {
            auto ccl = typeinfo_cast<CCLayer*>(obj);
            if (ccl != nullptr) {
                this->m_fields->m_mainLayer = ccl;
                break;
            }
        }
        return;
    }

    void loadPageFromUserInfo(GJUserScore* p0) {
        ProfilePage::loadPageFromUserInfo(p0);

        searchForLayer();

        auto gm = GameManager::get();
        if (p0->m_userID == gm->m_playerUserID) {
            Request::setCP(p0->m_creatorPoints);
            Request::generateNewTotalEXP();
        }
       
        auto playerMenu = this->m_fields->m_mainLayer->getChildByID("player-menu");
        auto iconBackground = this->m_fields->m_mainLayer->getChildByID("icon-background");
        auto statsMenu = this->m_fields->m_mainLayer->getChildByID("stats-menu");

        if (playerMenu == nullptr || iconBackground == nullptr || statsMenu == nullptr) {
            return;
        }
        
        if (Loader::get()->isModLoaded("bitz.customprofiles")) {
            iconBackground = this->m_fields->m_mainLayer->getChildByID("bitz.customprofiles/ccscale-icons");

            iconBackground->setContentSize({350.f, 54.f});
            iconBackground->setScale(0.98);

            auto commentBackgroundColor = this->m_fields->m_mainLayer->getChildByID("bitz.customprofiles/ccscale-comments");
            commentBackgroundColor->setScale(0.98);
            commentBackgroundColor->setContentWidth(350.f);
            commentBackgroundColor->setContentHeight(108.f);
            
        }

        if (!this->m_fields->m_determinePositions) {
            this->m_fields->m_newPlayerPositionY = playerMenu->getPositionY() - 12.f;
            this->m_fields->m_newIconBGPositionY = iconBackground->getPositionY() - 12.f;
            this->m_fields->m_newStatsPositionY = statsMenu->getPositionY() + 3.f; //shh
            this->m_fields->m_newStatsPositionX = statsMenu->getPositionX() + 25.f;
            
        }

        playerMenu->setPositionY(this->m_fields->m_newPlayerPositionY);
        iconBackground->setPositionY(this->m_fields->m_newIconBGPositionY);
        statsMenu->setPositionY(this->m_fields->m_newStatsPositionY);
        statsMenu->setPositionX(this->m_fields->m_newStatsPositionX);


        // auto leftMenu = this->m_fields->m_mainLayer->getChildByID("left-menu");
        // if (leftMenu != nullptr) {
        //     leftMenu->setPositionY(this->m_fields->m_newIconBGPositionY - 27.f);
        // }

        this->m_fields->m_determinePositions = true;

        if (this->m_fields->m_barAdded) {
            return;
        }

        auto barSpriteBack = CCSprite::create("GJ_progressBar_001.png");
        barSpriteBack->setScaleX(0.85f);
        barSpriteBack->setScaleY(0.7f);
        barSpriteBack->setPosition(statsMenu->getPosition());
        barSpriteBack->setPositionY(statsMenu->getPositionY() - 19.f);
        barSpriteBack->setPositionX(statsMenu->getPositionX());
        barSpriteBack->setColor({0, 0, 0});
        barSpriteBack->setOpacity(100);
        barSpriteBack->setZOrder(10);
        barSpriteBack->setID("tier-bar"_spr);
        this->m_fields->m_mainLayer->addChild(barSpriteBack);

        auto barSpriteTop = CCSprite::create("GJ_progressBar_001.png");
        barSpriteTop->setScaleX(0.988f);
        barSpriteTop->setScaleY(0.75f);
        barSpriteTop->setPosition({2, 10});
        barSpriteTop->setAnchorPoint({0, 0.5});
        barSpriteTop->setColor({0, 255, 0});
        barSpriteTop->setOpacity(255);
        barSpriteTop->setZOrder(11);

        CCPoint rectangle[4] = {
    		CCPoint(0, 0),
    		CCPoint(0, 20),
    		CCPoint(barSpriteTop->getScaledContentSize().width * -2.f, 20),
    		CCPoint(barSpriteTop->getScaledContentSize().width * -2.f, 0)
		};

        auto clippingNode = CCClippingNode::create();
        auto barMask = CCDrawNode::create();
        barMask->drawPolygon(rectangle, 4, ccc4FFromccc3B({0, 0, 0}), 0, ccc4FFromccc3B({0, 0, 0}));
        clippingNode->setStencil(barMask);
        clippingNode->addChild(barSpriteTop);
        barSpriteBack->addChild(clippingNode);

        // Calculate scale for StatsMenu
        float size1 = barSpriteBack->getScaledContentSize().width;
        float size2 = statsMenu->getScaledContentSize().width;
        float newScaleForStats = size1 / size2;
        statsMenu->setScale(newScaleForStats);

        // Create Text Labels
        int currentEXP = LevelHelper::calculateTotalEXP(this->m_score);
        int currentLevel = LevelHelper::getLevelFromEXP(currentEXP);
        int currentLevelEXP = LevelHelper::getEXPRequiredForLevel(currentLevel);
        int nextLevelEXP = LevelHelper::getEXPRequiredForLevel(currentLevel + 1);

        std::string progressString = fmt::format("{} / {}", currentEXP - currentLevelEXP, nextLevelEXP - currentLevelEXP);
        
        auto progressLabel = CCLabelBMFont::create(progressString.c_str(), "bigFont.fnt");
        barSpriteBack->addChild(progressLabel);
        progressLabel->setScaleX(0.34);
        progressLabel->setScaleY(0.4);
        progressLabel->setZOrder(15);
        progressLabel->setPosition(
            progressLabel->getPositionX() + barSpriteBack->getContentSize().width / 2,
            progressLabel->getPositionY() + (barSpriteBack->getContentSize().height / 2) + 0.5f
        );

        float percentageToNextLevel = (currentEXP - currentLevelEXP) / static_cast<double>((nextLevelEXP - currentLevelEXP)); 
        auto moveAction = CCEaseSineOut::create(CCMoveBy::create(1.f, {(barSpriteTop->getScaledContentSize().width + 2.f) * percentageToNextLevel, 0}));
        barMask->runAction(moveAction);

        auto badgeSprite = SpriteHelper::getBadgeFromLevel(currentLevel);
        badgeSprite->setID("tier-badge"_spr);
        badgeSprite->setZOrder(10);

        auto badgeLabel = CCLabelBMFont::create(std::to_string(currentLevel).c_str(), "bigFont.fnt");
        badgeLabel->setZOrder(11);
        badgeLabel->setScale((currentLevel >= 100 ? 0.3 : 0.4));
        badgeLabel->setPosition({badgeSprite->getContentSize().width / 2, badgeSprite->getContentSize().height / 2 + 1.f});
        badgeSprite->addChild(badgeLabel);

        auto badgeShadow = CCSprite::createWithSpriteFrameName("shadow.png"_spr);
        badgeShadow->setZOrder(-5);
        badgeShadow->setScale(0.9);
        badgeShadow->setPosition({badgeSprite->getContentSize().width / 2, badgeSprite->getContentSize().height / 2});
        badgeShadow->setOpacity(100);
        badgeSprite->addChild(badgeShadow);

        if (!(Mod::get()->getSettingValue<bool>("most-exp-icon"))) {
            auto badgeMostEXPIcon = SpriteHelper::getMostEXPIcon(this->m_score);
            if (badgeMostEXPIcon != nullptr) {
                badgeMostEXPIcon->setZOrder(12);
                badgeMostEXPIcon->setID("tier-badge-stat"_spr);
                badgeMostEXPIcon->setScale(0.5);
                badgeMostEXPIcon->setPosition({badgeSprite->getContentSize().width / 2, 17.5f});
                badgeSprite->addChild(badgeMostEXPIcon);
            }
        }

        auto badgeMenu = CCMenu::create();
        auto badgeButton = CCMenuItemSpriteExtra::create(badgeSprite, this, menu_selector(BP_ProfilePage::onBadgePress));

        badgeMenu->setPositionY(barSpriteBack->getPositionY() + 5.f);
        badgeMenu->setPositionX(barSpriteBack->getPositionX() - (barSpriteBack->getScaledContentSize().width / 2) - 33.f);
        badgeMenu->setScale(0.9);
        badgeMenu->setZOrder(50);
        badgeMenu->setContentSize(badgeSprite->getContentSize());
        badgeMenu->addChild(badgeButton);
        badgeMenu->setID("tier-badge-menu"_spr);

        auto badgeGlow = SpriteHelper::getGlowFromLevel(currentLevel);
        if (badgeGlow != nullptr && currentLevel <= 350) {
            badgeGlow->setZOrder(-3);
            badgeGlow->setPosition({badgeSprite->getContentSize().width / 2, badgeSprite->getContentSize().height / 2});
            badgeGlow->setScaleX(0.975);
            badgeGlow->setScaleY(0.95);
            badgeSprite->addChild(badgeGlow);

            ccBlendFunc blending = {GL_ONE, GL_ONE};
            badgeGlow->setBlendFunc(blending);

            if (currentLevel >= 200 && currentLevel <= 224)
                badgeGlow->setColor({ 255, 156, 110 });
            if (currentLevel >= 225 && currentLevel <= 249)
                badgeGlow->setColor({ 90, 255, 148 });
            if (currentLevel >= 250 && currentLevel <= 274)
                badgeGlow->setColor({ 65, 239, 255 });
            if (currentLevel >= 275 && currentLevel <= 299)
                badgeGlow->setColor({ 245, 108, 255 });
            
            auto fadein = CCEaseSineInOut::create(CCFadeTo::create(0.9f, 50));
            auto fadeout = CCEaseSineInOut::create(CCFadeTo::create(0.9f, 255));
            auto sequence = CCSequence::create(fadein, fadeout, nullptr);
            auto repeatSequence = CCRepeatForever::create(sequence);

            badgeGlow->runAction(repeatSequence);
        }

        ccColor3B barColor = {0, 255, 0};
        if (!(Mod::get()->getSettingValue<bool>("bar-color"))) {
            barColor = SpriteHelper::getColorFromLevel(currentLevel);

            if (currentLevel >= 300 && currentLevel <= 349) {
                auto tint1 = CCTintTo::create(0.3f, 255, 200, 150);
                auto tint2 = CCTintTo::create(0.3f, 255, 255, 150);
                auto tint3 = CCTintTo::create(0.3f, 200, 255, 150);
                auto tint4 = CCTintTo::create(0.3f, 150, 255, 150);
                auto tint5 = CCTintTo::create(0.3f, 150, 255, 200);
                auto tint6 = CCTintTo::create(0.3f, 150, 255, 255);
                auto tint7 = CCTintTo::create(0.3f, 150, 200, 255);
                auto tint8 = CCTintTo::create(0.3f, 150, 150, 255);
                auto tint9 = CCTintTo::create(0.3f, 200, 150, 255);
                auto tint10 = CCTintTo::create(0.3f, 255, 150, 255);
                auto tint11 = CCTintTo::create(0.3f, 255, 150, 150);

                auto tintseq = CCSequence::create(tint1, tint2, tint3, tint4, tint5, tint6, tint7, tint8, tint9, tint10, tint11, nullptr);
                auto tintrepeatseq = CCRepeatForever::create(tintseq);

                //badgeGlow->runAction(tintrepeatseq);
                barSpriteTop->runAction(tintrepeatseq);
            }
        }

        barSpriteTop->setColor(barColor);

        if (p0->m_userID == gm->m_playerUserID) {
            auto infoIconBtnSpr = CircleButtonSprite::createWithSpriteFrameName("view.png"_spr);
            infoIconBtnSpr->setScale(0.2f);
            auto infoIconBtn = CCMenuItemSpriteExtra::create(
                infoIconBtnSpr, this, menu_selector(BP_ProfilePage::onAnimPress)
            );

            auto infoIconMenu = CCMenu::create();
            infoIconMenu->addChild(infoIconBtn);
            this->m_fields->m_mainLayer->addChild(infoIconMenu);
            infoIconMenu->setPosition({badgeMenu->getPositionX() + 41.f, badgeMenu->getPositionY() - 5.f});
            infoIconMenu->setZOrder(20);
            infoIconMenu->setID("tier-anim-menu"_spr);
        }

        // User is likely a cheater
        if (currentLevel >= 350) {
            badgeMenu->setColor({0, 0, 0});
            barSpriteTop->setVisible(false);
            progressLabel->setVisible(false);
            badgeSprite->setVisible(false);
        }

        this->m_fields->m_mainLayer->addChild(badgeMenu);
        this->m_fields->m_barAdded = true;

        Request::performCPRequest();
        Request::generateNewTotalEXP();
    }

    void onBadgePress(CCObject* sender) {
        int currentEXP = LevelHelper::calculateTotalEXP(this->m_score);
        int currentLevel = LevelHelper::getLevelFromEXP(currentEXP);
        int currentLevelEXP = LevelHelper::getEXPRequiredForLevel(currentLevel);
        int nextLevelEXP = LevelHelper::getEXPRequiredForLevel(currentLevel + 1);

        float percentageToNextLevel = (int)((currentEXP - currentLevelEXP) / static_cast<double>((nextLevelEXP - currentLevelEXP)) * 10000 + 0.5);
        float perc = (float)percentageToNextLevel / 100;

        std::string title = fmt::format("Tier {}", currentLevel);
        std::string desc = fmt::format(
            "<cy>Stars EXP: </c>{}\n<cl>Moons EXP: </c>{}\n<cf>Diamonds EXP: </c>{}\n<co>Secret Coins EXP: </c>{}\n<cc>User Coins EXP:</c> {}\n<cr>Demons EXP: </c>{}\n<cg>Creator Points EXP: </c>{}\n\n<cp>Total EXP: </c>{}\n<ca>Progress: </c>{}%",
        this->m_score->m_stars * STAR_EXP,
        this->m_score->m_moons * MOON_EXP,
        this->m_score->m_diamonds * DIAMOND_EXP,
        this->m_score->m_secretCoins * GOLD_COIN_EXP,
        this->m_score->m_userCoins * USER_COIN_EXP,
        this->m_score->m_demons * DEMON_EXP,
        this->m_score->m_creatorPoints * CP_EXP,
        currentEXP,
        perc);

        if (this->m_accountID == 150529) {
            desc = "cant you just play the game and enjoy that? if you guys wanna watch meaningless numbers go up you can play cookie clicker or something. go play runescape. those games are all about shit like this";
        }
        
        auto alert = FLAlertLayer::create(title.c_str(), desc.c_str(), "OK");
        alert->show();
    }

    void onAnimPress(CCObject* sender) {
        
        int newEXP = Request::currentTotalEXP();
        int currentLevel = LevelHelper::getLevelFromEXP(newEXP);
        int currentLevelEXP = LevelHelper::getEXPRequiredForLevel(currentLevel);
        int nextLevel = currentLevel + 1;
        int nextLevelEXP = LevelHelper::getEXPRequiredForLevel(nextLevel);

        auto scene = CCDirector::sharedDirector()->getRunningScene();
        CCLayer* parentLayer = nullptr;
        CCObject* obj;

        CCARRAY_FOREACH(scene->getChildren(), obj) {
            auto ccl = typeinfo_cast<CCLayer*>(obj);
            if (ccl != nullptr) {
                parentLayer = ccl;
                break;
            }
        }

        TierBarPopup::createPopupSubroutine(scene, currentLevelEXP, newEXP, 4.5f);
        if (parentLayer != nullptr) {
            auto popup = LevelUpPopup::create(0, currentLevel);
            popup->m_scene = parentLayer;
            popup->show();
        }

        onClose(this);
    }
};