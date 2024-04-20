#pragma once

#include <Geode/Geode.hpp>
#include "LevelHelper.hpp"
#include "SpriteHelper.hpp"

using namespace geode::prelude;

class TierBarPopup {
	public:

		inline static CCLabelBMFont* m_progressLabel = nullptr;
		inline static CCLabelBMFont* m_gainedEXPLabel = nullptr;
		inline static CCScale9Sprite* m_popup = nullptr;
		inline static int m_expMaxDisplayVal = 0;
		inline static float m_timePerEXPTick = 0;
		inline static int m_expDecayValue = 1;
		inline static int m_maxEXP = 0;

		inline static int m_currentBarValue = 0;
		inline static int m_currentGainedEXPValue = 0;

		inline static bool m_popupExists = false;

		static void createPopup(CCScene* scene, int originalEXP, int newEXP, float delay);
		void soundCallback();
		void incrementEXPByOne();
		void removePopup();
		static void createPopupSubroutine(CCScene* scene, int originalEXP, int newEXP, float delay);
};