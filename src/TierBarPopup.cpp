#include "TierBarPopup.hpp"

void TierBarPopup::createPopup(CCScene* scene, int originalEXP, int newEXP, float delay) {

    if (TierBarPopup::m_popupExists) {
        return;
    }
    TierBarPopup::m_popupExists = true;

    m_expMaxDisplayVal = 0;
    m_timePerEXPTick = 0;
    m_expDecayValue = 1;

    m_currentGainedEXPValue = 0;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto background = CCScale9Sprite::create("square02_001.png");
    background->setContentSize({winSize.width * 0.9f, 65.f});
    background->setPosition({winSize.width / 2, 40.f - 80.f});
    background->setZOrder(1);
    background->setOpacity((delay > 1.0f ? 0 : 150));
    scene->addChild(background);

    background->setID("tier-popup"_spr);
    TierBarPopup::m_popup = background;

    auto barSpriteBack = CCSprite::create("GJ_progressBar_001.png");
    barSpriteBack->setScaleX(1.02f);
    barSpriteBack->setScaleY(0.8f);
    barSpriteBack->setColor({0, 0, 0});
    barSpriteBack->setOpacity(200);
    barSpriteBack->setZOrder(10);
    barSpriteBack->setID("tier-bar-menu"_spr);
    background->addChild(barSpriteBack);

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

    // Create Text Labels
    int currentEXP = originalEXP;
    int currentLevel = LevelHelper::getLevelFromEXP(currentEXP);
    int currentLevelEXP = LevelHelper::getEXPRequiredForLevel(currentLevel);
    int nextLevelEXP = LevelHelper::getEXPRequiredForLevel(currentLevel + 1);

    m_currentBarValue = originalEXP - currentLevelEXP;
    TierBarPopup::m_expMaxDisplayVal = nextLevelEXP - currentLevelEXP;

    int diffEXP = newEXP - originalEXP;
    TierBarPopup::m_maxEXP = m_currentBarValue + diffEXP;

    // Limit number of EXP inc/decs to 250
    if (diffEXP > 250) {
        TierBarPopup::m_expDecayValue = diffEXP / 250;
    }

    TierBarPopup::m_timePerEXPTick = 1.5f / (diffEXP / m_expDecayValue);

    //Notification::create(fmt::format("{}", m_timePerEXPTick))->show();

    std::string progressString = fmt::format("{} / {}", currentEXP - currentLevelEXP, nextLevelEXP - currentLevelEXP);
    TierBarPopup::m_progressLabel = CCLabelBMFont::create(progressString.c_str(), "bigFont.fnt");
    auto progressLabel = TierBarPopup::m_progressLabel;
    barSpriteBack->addChild(progressLabel);
    progressLabel->setScaleX(0.33);
    progressLabel->setScaleY(0.4);
    progressLabel->setZOrder(15);
    progressLabel->setPosition(
        progressLabel->getPositionX() + barSpriteBack->getContentSize().width / 2,
        progressLabel->getPositionY() + (barSpriteBack->getContentSize().height / 2) + 0.5f
    );
    float percentageToNextLevel = (currentEXP - currentLevelEXP) / static_cast<double>((nextLevelEXP - currentLevelEXP)); 
    auto moveAction = CCEaseSineOut::create(CCMoveBy::create(0.f, {(barSpriteTop->getScaledContentSize().width + 2.f) * percentageToNextLevel, 0}));
    
    float percentageToNextLevel2 = (newEXP - currentLevelEXP) / static_cast<double>((nextLevelEXP - currentLevelEXP)) - percentageToNextLevel; 
    auto moveAction2 = CCEaseSineOut::create(CCMoveBy::create(((diffEXP * TierBarPopup::m_timePerEXPTick) / m_expDecayValue), {(barSpriteTop->getScaledContentSize().width + 2.f) * percentageToNextLevel2, 0}));

    TierBarPopup::m_currentGainedEXPValue = diffEXP;
    std::string gainedEXPString = fmt::format("+{} EXP", diffEXP);
    TierBarPopup::m_gainedEXPLabel = CCLabelBMFont::create(gainedEXPString.c_str(), "bigFont.fnt");
    auto gainedEXPLabel = TierBarPopup::m_gainedEXPLabel;
    barSpriteBack->addChild(gainedEXPLabel);
    gainedEXPLabel->setZOrder(15);
    gainedEXPLabel->setColor({100, 255, 100});
    gainedEXPLabel->setScale(0);

    auto expColor1 = CCTintTo::create(0.3f, 100, 255, 100);
    auto expColor2 = CCTintTo::create(0.3f, 200, 255, 200);

    auto expColorSequence = CCSequence::create(expColor2, expColor1, nullptr);
    auto expColorRseq = CCRepeatForever::create(expColorSequence);

    auto scaleUpSequence = CCSequence::create(CCDelayTime::create(0.4f), CCEaseExponentialOut::create(CCScaleTo::create(1.f, 0.28f, 0.33f)), nullptr);
    gainedEXPLabel->runAction(scaleUpSequence);
    gainedEXPLabel->runAction(expColorRseq);

    gainedEXPLabel->setPosition({25, -10});

    auto badgeSprite = SpriteHelper::getBadgeFromLevel(currentLevel);
    badgeSprite->setID("tier-badge-menu"_spr);
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

    ccBlendFunc blending = {GL_ONE, GL_ONE};
    auto badgeShine = CCSprite::createWithSpriteFrameName("shineBurst_001.png");
    badgeShine->setZOrder(-4);
    badgeShine->setOpacity(150);
    badgeShine->setPosition({badgeSprite->getContentSize().width / 2, badgeSprite->getContentSize().height / 2});
    badgeShine->setBlendFunc(blending);
    badgeShine->setScale(1.3f);
    badgeShine->setColor(SpriteHelper::getColorFromLevel(currentLevel));
    
    auto shineRot = CCRotateTo::create(15.f, 2160.f);
    badgeShine->runAction(shineRot);

    badgeSprite->addChild(badgeShine);

    auto badgeGlow = SpriteHelper::getGlowFromLevel(currentLevel);
    if (badgeGlow != nullptr && currentLevel <= 350) {
        badgeGlow->setZOrder(-3);
        badgeGlow->setPosition({badgeSprite->getContentSize().width / 2, badgeSprite->getContentSize().height / 2});
        badgeGlow->setScaleX(0.975);
        badgeGlow->setScaleY(0.95);
        badgeSprite->addChild(badgeGlow);

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

    background->addChild(badgeSprite);
    badgeSprite->setPosition({40, background->getContentHeight() / 2});
    barSpriteBack->setPosition({250, background->getContentHeight() / 2});

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

    // ACTIONS
    // SLIDE IN
    auto bgwait0 = CCDelayTime::create(0.33f + delay);
    auto soundFunc = CCCallFunc::create(nullptr, callfunc_selector(TierBarPopup::soundCallback));
    auto raiseAction = CCEaseExponentialOut::create(CCMoveBy::create(0.8f, {0, 80}));
    auto bgwait1 = CCDelayTime::create(0.7f + ((diffEXP * TierBarPopup::m_timePerEXPTick) / m_expDecayValue));
    auto lowerAction = CCEaseExponentialIn::create(CCMoveBy::create(0.5f, {0, -80}));
    auto cleanup = CCCallFunc::create(nullptr, callfunc_selector(TierBarPopup::removePopup));
    auto bgSequence = CCSequence::create(bgwait0, soundFunc, raiseAction, bgwait1, lowerAction, cleanup, nullptr);
    background->runAction(bgSequence);

    // BAR MOVEMENT
    auto barwait0 = CCDelayTime::create(1.2f + delay);
    auto barSequence = CCSequence::create(moveAction, barwait0, moveAction2, nullptr);
    barMask->runAction(barSequence);

    // LABEL CHANGE

    auto scalein = CCEaseInOut::create(CCScaleBy::create(0.3f, 1.1f), 2.f);
    auto scaleout = CCEaseInOut::create(CCScaleBy::create(0.3f, 1 / 1.1f), 2.f);

    auto scalein2 = CCEaseInOut::create(CCScaleBy::create(0.3f, 1.1f), 2.f);
    auto scaleout2 = CCEaseInOut::create(CCScaleBy::create(0.3f, 1 / 1.1f), 2.f);

    auto labelSequenceArray = CCArray::create();
    labelSequenceArray->addObject(CCDelayTime::create(1.2f + delay));
    for (int i = 0; i < diffEXP; i += m_expDecayValue) {
        labelSequenceArray->addObject(CCCallFunc::create(nullptr, callfunc_selector(TierBarPopup::incrementEXPByOne)));
        labelSequenceArray->addObject(CCDelayTime::create(TierBarPopup::m_timePerEXPTick));
    }
    labelSequenceArray->addObject(CCDelayTime::create(0.1f));
    labelSequenceArray->addObject(scalein2);
    labelSequenceArray->addObject(scaleout2);
    auto labelSequence = CCSequence::create(labelSequenceArray);
    progressLabel->runAction(labelSequence);

    // SCALE CHANGE
    auto scaleWait = CCDelayTime::create(2.8f + delay);

    auto scaleSequence = CCSequence::create(scaleWait, scalein, scaleout, nullptr);
    badgeSprite->runAction(scaleSequence);

}

void TierBarPopup::soundCallback() {
    auto engine = FMODAudioEngine::sharedEngine();
    engine->playEffect("achievement_01.ogg");
}

void TierBarPopup::incrementEXPByOne() {
    int currValue = TierBarPopup::m_currentBarValue;
    currValue += TierBarPopup::m_expDecayValue;

    if (currValue > m_maxEXP) {
        currValue = m_maxEXP;
    }

    std::string newStringVal = fmt::format("{} / {}", currValue, TierBarPopup::m_expMaxDisplayVal);
    TierBarPopup::m_progressLabel->setCString(newStringVal.c_str());

    int currValue2 = TierBarPopup::m_currentGainedEXPValue;
    currValue2 -= TierBarPopup::m_expDecayValue;
    std::string newStringVal2 = fmt::format("+{} EXP", currValue2);
    TierBarPopup::m_gainedEXPLabel->setCString(newStringVal2.c_str());

    if (currValue2 <= 0) {
        TierBarPopup::m_gainedEXPLabel->setOpacity(0);
    }

    TierBarPopup::m_currentBarValue = currValue;
    TierBarPopup::m_currentGainedEXPValue = currValue2;
}

void TierBarPopup::removePopup() {
    TierBarPopup::m_popup->removeFromParentAndCleanup(true);
    TierBarPopup::m_popupExists = false;
}

void TierBarPopup::createPopupSubroutine(CCScene* scene, int originalEXP, int newEXP, float delay) {

    if (newEXP - originalEXP == 0) {
        return;
    }

    createPopup(scene, originalEXP, newEXP, delay);
}