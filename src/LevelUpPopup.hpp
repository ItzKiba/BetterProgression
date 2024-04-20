#pragma once

#include <Geode/Geode.hpp>
#include <Geode/fmod/fmod.hpp>
#include "SpriteHelper.hpp"
#include "ParticleManager.hpp"
#include "TierBarPopup.hpp"

using namespace geode::prelude;

class LevelUpPopup : public FLAlertLayer {
    public:
        CCLayer* m_parentLayer;
        float m_originalVolume;

        bool init(int level, int newLevel);
        static LevelUpPopup* create(int level, int newLevel); 

        virtual void onClose(CCObject* sender);
        void keyBackClicked();
        void registerWithTouchDispatcher();

        CCMoveTo* generateShakeAction(CCPoint originalPos, float xyOffset, float duration);
        void soundCallback1();

        void inwardParticlesEnable(CCNode* sender);

        void outwardParticlesEnable(CCNode* sender);

        void bottomParticlesEnable(CCNode* sender);

        void createInnerPopup();

        void enableKeypad();
};