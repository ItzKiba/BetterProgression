#include <Geode/modify/GameManager.hpp>
#include "Manager.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
	auto manager = Manager::getSharedInstance();
	manager->originalVolume = FMODAudioEngine::sharedEngine()->m_musicVolume;
	manager->isWherwin = (GameManager::get()->m_playerUserID.value() == 3864233 || GameManager::get()->m_playerUserID.value() == 10709102);
	//log::info("{}", Manager::getSharedInstance()->originalVolume);
}

class $modify(MyGameManager, GameManager) {
	void encodeDataTo(DS_Dictionary* p0) {
		auto origVol = Manager::getSharedInstance()->originalVolume;
		auto engine = FMODAudioEngine::sharedEngine();
		m_bgVolume = origVol;
		engine->m_musicVolume = origVol;
		engine->m_backgroundMusicChannel->setVolume(origVol);
		log::info("{}", Manager::getSharedInstance()->originalVolume);
		GameManager::encodeDataTo(p0);
	}
};