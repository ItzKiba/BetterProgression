#include <Geode/modify/GameManager.hpp>
#include "Manager.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
	Manager::getSharedInstance()->originalVolume = FMODAudioEngine::sharedEngine()->m_musicVolume;
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