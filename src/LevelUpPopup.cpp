#include "LevelUpPopup.hpp"
#include "Manager.hpp"

LevelUpPopup* LevelUpPopup::create(int level, int newLevel) {
	auto ret = new LevelUpPopup();
	if (ret && ret->init(level, newLevel)) {
		ret->autorelease();
	} else {
		delete ret;
		ret = nullptr;
	}
	return ret;
}

bool LevelUpPopup::init(int level, int newLevel) {
	if (!FLAlertLayer::init(0xC8)) {
		return false;
	}

	Loader::get()->queueInMainThread([this] {
		auto parentLayer = typeinfo_cast<CCLayer*>(this->getParent());
		
		// ORIGINAL CODE:
		// m_parentLayer = parentLayer;

		// NEW CODE
		if (parentLayer->getID() == "prism-icon") {
			auto parentLayer2 = typeinfo_cast<CCLayer*>(parentLayer->getParent());
			m_parentLayer = parentLayer2;
		} else {
			m_parentLayer = parentLayer;
		}
		///////////

		m_parentLayer->setKeypadEnabled(false);
		this->setKeypadEnabled(false);

		auto enablekeypad_array = CCArray::create();
		enablekeypad_array->addObject(CCDelayTime::create(8.0f));
		enablekeypad_array->addObject(CCCallFunc::create(this, callfunc_selector(LevelUpPopup::enableKeypad)));

		auto ek_seq = CCSequence::create(enablekeypad_array);
		this->runAction(ek_seq);
		
	});

	ccBlendFunc blending = {GL_ONE, GL_ONE};
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto sprite1_main = createFullSprite(level);
	sprite1_main->setPosition(winSize / 2);
	sprite1_main->setScale(0);
	m_mainLayer->addChild(sprite1_main);

	auto badgeShine1_1 = CCSprite::createWithSpriteFrameName("shineBurst_001.png");
	badgeShine1_1->setZOrder(-4);
	badgeShine1_1->setPosition({winSize / 2});
	badgeShine1_1->setBlendFunc(blending);
	badgeShine1_1->setScale(6.f);
	badgeShine1_1->setColor(SpriteHelper::getColorFromLevel(level));
	auto shineRot1 = CCRotateTo::create(999.f, 99900.f);
	badgeShine1_1->runAction(shineRot1);
	m_mainLayer->addChild(badgeShine1_1);

	auto badgeShine1_2 = CCSprite::createWithSpriteFrameName("shineBurst_001.png");
	badgeShine1_2->setZOrder(-4);
	badgeShine1_2->setPosition({winSize / 2});
	badgeShine1_2->setBlendFunc(blending);
	badgeShine1_2->setScale(3.5f);
	badgeShine1_2->setColor(SpriteHelper::getColorFromLevel(level));
	auto shineRot2 = CCRotateTo::create(999.f, -2.5*99900.f);
	badgeShine1_2->runAction(shineRot2);
	m_mainLayer->addChild(badgeShine1_2);

	badgeShine1_1->setOpacity(0);
	badgeShine1_2->setOpacity(0);

	auto shineBall = CCSprite::createWithSpriteFrameName("shineball.png"_spr);
	shineBall->setZOrder(99);
	shineBall->setPosition({winSize / 2});
	shineBall->setBlendFunc(blending);
	shineBall->setScale(7.f);
	m_mainLayer->addChild(shineBall);

	shineBall->setOpacity(0);

	auto inwardParticles = ParticleManager::inwardParticles(100, SpriteHelper::getColorFromLevel(level));
	inwardParticles->setVisible(false);
	inwardParticles->setZOrder(-4);
	inwardParticles->setPosition(winSize / 2);
	m_mainLayer->addChild(inwardParticles);

	auto outwardParticles = ParticleManager::outwardParticles(69, SpriteHelper::getColorFromLevel(newLevel));
	outwardParticles->setVisible(false);
	outwardParticles->setZOrder(-4);
	outwardParticles->setPosition(winSize / 2);
	m_mainLayer->addChild(outwardParticles);

	auto bottomParticles = ParticleManager::bottomParticles(50, SpriteHelper::getColorFromLevel(newLevel));
	bottomParticles->setPosition({winSize.width / 2, -5});
	bottomParticles->setVisible(false);
	m_mainLayer->addChild(bottomParticles);

	//

	auto sprite2_main = createFullSprite(newLevel);
	sprite2_main->setPosition({2000, 2000});
	sprite2_main->setScale(4.0);

	// 

	m_mainLayer->addChild(sprite2_main);

	auto closeMenu = CCMenu::create();
	closeMenu->setPosition({24, winSize.height + (100 - 24)});
	auto closeBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"), this, menu_selector(LevelUpPopup::onClose));
	closeMenu->addChild(closeBtn);
	closeMenu->setScale(0.75);
	closeMenu->setAnchorPoint({0, 0});
	this->addChild(closeMenu);

	closeMenu->setOpacity(0);

	auto closeBtn_array = CCArray::create();
	closeBtn_array->addObject(CCDelayTime::create(8.0f));
	closeBtn_array->addObject(CCMoveBy::create(0, {0, -100}));
	closeBtn_array->addObject(CCFadeTo::create(1.f, 255));

	auto closeBtn_seq = CCSequence::create(closeBtn_array);
	closeMenu->runAction(closeBtn_seq);

	//

	auto tierUp = CCSprite::createWithSpriteFrameName("tierup.png"_spr);
	tierUp->setPosition({winSize.width / 2, winSize.height / 2 - 70.f});
	tierUp->setScale(0);
	tierUp->setZOrder(10);
	m_mainLayer->addChild(tierUp);

	auto tierUp_array1 = CCArray::create();
	tierUp_array1->addObject(CCDelayTime::create(3.f));
	tierUp_array1->addObject(CCEaseBackOut::create(CCScaleTo::create(1.f, 0.9f)));
	auto tierUp_seq1 = CCSequence::create(tierUp_array1);
	tierUp->runAction(tierUp_seq1);

	//

	auto engine = FMODAudioEngine::sharedEngine();
	auto manager = Manager::getSharedInstance();
	m_originalVolume = engine->m_musicVolume;
	manager->orignalVolume = engine->m_musicVolume;
	float newVolume = m_originalVolume / 4;

	manager->volChangeNotFromTierUp = false;
	engine->m_globalChannel->setVolume(newVolume);
	engine->m_backgroundMusicChannel->setVolume(newVolume);
	manager->volChangeNotFromTierUp = true;

	// ACTIONS

	auto sprite1_array1 = CCArray::create();
	sprite1_array1->addObject(CCDelayTime::create(0.6f));
	sprite1_array1->addObject(CCEaseExponentialOut::create(CCScaleTo::create(2.4f, 2.5f)));

	auto sprite1_seq1 = CCSequence::create(sprite1_array1);
	sprite1_main->runAction(sprite1_seq1);

	auto sprite1_array3 = CCArray::create();
	sprite1_array3->addObject(CCDelayTime::create(2.0f));
	sprite1_array3->addObject(CCEaseExponentialIn::create(CCScaleTo::create(1.0f, 4.0f)));

	auto sprite1_seq3 = CCSequence::create(sprite1_array3);
	sprite1_main->runAction(sprite1_seq3);
	
	/////////////////

	auto sprite1_array2 = CCArray::create();
	sprite1_array2->addObject(CCDelayTime::create(0.6f));

	sprite1_array2->addObject(CCCallFunc::create(nullptr, callfunc_selector(LevelUpPopup::soundCallback1)));
	for (unsigned int i = 0; i < 60; i++) {
		sprite1_array2->addObject(generateShakeAction({winSize / 2}, 1.0f, 0.01f));
	}
	for (unsigned int i = 0; i < 60; i++) {
		sprite1_array2->addObject(generateShakeAction({winSize / 2}, 2.0f, 0.01f));
	}
	for (unsigned int i = 0; i < 60; i++) {
		sprite1_array2->addObject(generateShakeAction({winSize / 2}, 3.0f, 0.01f));
	}
	for (unsigned int i = 0; i < 60; i++) {
		sprite1_array2->addObject(generateShakeAction({winSize / 2}, 4.0f, 0.01f));
	}

	sprite1_array2->addObject(CCMoveBy::create(0.0f, {2000, 2000}));
	auto sprite1_seq2 = CCSequence::create(sprite1_array2);
	sprite1_main->runAction(sprite1_seq2);

	//////////////////

	auto sprite2_array1 = CCArray::create();
	sprite2_array1->addObject(CCDelayTime::create(3.0f));
	sprite2_array1->addObject(CCMoveTo::create(0.0f, winSize / 2));
	sprite2_array1->addObject(CCEaseExponentialOut::create(CCScaleTo::create(1.5f, 2.5f)));

	auto sprite2_seq1 = CCSequence::create(sprite2_array1);
	sprite2_main->runAction(sprite2_seq1);

	//////////////////

	auto newColor = SpriteHelper::getColorFromLevel(newLevel);

	auto shine_array1 = CCArray::create();
	shine_array1->addObject(CCDelayTime::create(1.2f));
	shine_array1->addObject(CCFadeTo::create(1.8f, 30));
	shine_array1->addObject(CCTintTo::create(0.2f, newColor.r, newColor.g, newColor.b));
	if (newLevel >= 300 && newLevel <= 349) {
		shine_array1->addObject(CCTintTo::create(0.2f, 255, 255, 255));
	}
	auto shine1_seq = CCSequence::create(shine_array1);
	badgeShine1_1->runAction(shine1_seq);

	auto shine_array2 = CCArray::create();
	shine_array2->addObject(CCDelayTime::create(0.6f));
	shine_array2->addObject(CCFadeTo::create(2.4f, 150));
	shine_array2->addObject(CCTintTo::create(0.2f, newColor.r, newColor.g, newColor.b));
	if (newLevel >= 300 && newLevel <= 349) {
		shine_array2->addObject(CCTintTo::create(0.2f, 255, 255, 255));
	}
	auto shine2_seq = CCSequence::create(shine_array2);
	badgeShine1_2->runAction(shine2_seq);

	///////////////////

	auto shineBall_array = CCArray::create();
	shineBall_array->addObject(CCDelayTime::create(0.6f));
	shineBall_array->addObject(CCFadeTo::create(2.25f, 50));
	shineBall_array->addObject(CCFadeTo::create(0.15f, 255));
	shineBall_array->addObject(CCDelayTime::create(0.1f));
	shineBall_array->addObject(CCFadeTo::create(1.5f, 0));

	auto shineBall_seq = CCSequence::create(shineBall_array);
	shineBall->runAction(shineBall_seq);

	////////////////////

	auto inward_array = CCArray::create();
	inward_array->addObject(CCDelayTime::create(0.4f));
	inward_array->addObject(CCCallFuncN::create(nullptr, callfuncN_selector(LevelUpPopup::inwardParticlesEnable)));
	inward_array->addObject(CCDelayTime::create(0.6f));
	inward_array->addObject(CCEaseExponentialIn::create(CCScaleTo::create(2.0f, 0)));

	auto inward_seq = CCSequence::create(inward_array);
	inwardParticles->runAction(inward_seq);

	auto outward_array = CCArray::create();
	outward_array->addObject(CCDelayTime::create(3.0f));
	outward_array->addObject(CCCallFuncN::create(nullptr, callfuncN_selector(LevelUpPopup::outwardParticlesEnable)));

	auto outward_seq = CCSequence::create(outward_array);
	outwardParticles->runAction(outward_seq);

	auto bottom_array = CCArray::create();
	bottom_array->addObject(CCDelayTime::create(3.0f));
	bottom_array->addObject(CCCallFuncN::create(nullptr, callfuncN_selector(LevelUpPopup::bottomParticlesEnable)));

	auto bottom_seq = CCSequence::create(bottom_array);
	bottomParticles->runAction(bottom_seq);

	//////////////////

	auto moveup_array1 = CCArray::create();
	moveup_array1->addObject(CCDelayTime::create(4.5f));
	moveup_array1->addObject(CCEaseExponentialInOut::create(CCMoveBy::create(1.0f, {0, 50})));
	auto moveup_array2 = CCArray::create();
	moveup_array2->addObject(CCDelayTime::create(4.5f));
	moveup_array2->addObject(CCEaseExponentialInOut::create(CCMoveBy::create(1.0f, {0, 50})));
	auto moveup_array3 = CCArray::create();
	moveup_array3->addObject(CCDelayTime::create(4.5f));
	moveup_array3->addObject(CCEaseExponentialInOut::create(CCMoveBy::create(1.0f, {0, 50})));
	auto moveup_array4 = CCArray::create();
	moveup_array4->addObject(CCDelayTime::create(4.5f));
	moveup_array4->addObject(CCEaseExponentialInOut::create(CCMoveBy::create(1.0f, {0, 50})));
	auto moveup_array5 = CCArray::create();
	moveup_array5->addObject(CCDelayTime::create(4.5f));
	moveup_array5->addObject(CCEaseExponentialInOut::create(CCMoveBy::create(1.0f, {0, 50})));

	auto moveup_seq1 = CCSequence::create(moveup_array1);
	auto moveup_seq2 = CCSequence::create(moveup_array2);
	auto moveup_seq3 = CCSequence::create(moveup_array3);
	auto moveup_seq4 = CCSequence::create(moveup_array4);
	auto moveup_seq5 = CCSequence::create(moveup_array5);

	badgeShine1_1->runAction(moveup_seq1);
	badgeShine1_2->runAction(moveup_seq2);
	sprite2_main->runAction(moveup_seq3);
	outwardParticles->runAction(moveup_seq4);
	tierUp->runAction(moveup_seq5);

	///////////////////

	return true;
}

void LevelUpPopup::keyBackClicked() {
	onClose(nullptr);
}

void LevelUpPopup::onClose(CCObject* sender) {
	setKeypadEnabled(false);
	m_parentLayer->setKeypadEnabled(true);
	auto engine = FMODAudioEngine::sharedEngine();
	engine->m_globalChannel->setVolume(m_originalVolume);
	engine->m_backgroundMusicChannel->setVolume(m_originalVolume);
	Manager::getSharedInstance()->volChangeNotFromTierUp = true;
	removeFromParentAndCleanup(true);
}

void LevelUpPopup::registerWithTouchDispatcher() {
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -250, true);
}

CCSprite* LevelUpPopup::createFullSprite(int currentLevel) {

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

	return badgeSprite;
}

CCMoveTo* LevelUpPopup::generateShakeAction(CCPoint originalPos, float xyOffset, float duration) {
	float dx = CCRANDOM_MINUS1_1() * xyOffset;
	float dy = CCRANDOM_MINUS1_1() * xyOffset;

	return CCMoveTo::create(duration, {originalPos.x + dx, originalPos.y + dy});
}

void LevelUpPopup::soundCallback1() {
	auto engine = FMODAudioEngine::sharedEngine();
	engine->playEffect("unlockPath.ogg");
}

void LevelUpPopup::inwardParticlesEnable(CCNode* sender) {
	sender->setVisible(true);
}

void LevelUpPopup::outwardParticlesEnable(CCNode* sender) {
	sender->setVisible(true);
}

void LevelUpPopup::bottomParticlesEnable(CCNode* sender) {
	sender->setVisible(true);
}

void LevelUpPopup::enableKeypad() {
	this->setKeypadEnabled(true);
}