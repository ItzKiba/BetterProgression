#include <Geode/modify/GameManager.hpp>
#include "Manager.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
	Manager::getSharedInstance()->orignalVolume = FMODAudioEngine::sharedEngine()->m_musicVolume;
}

class $modify(MyGameManager, GameManager) {
	void encodeDataTo(DS_Dictionary* p0) {
		auto origVol = Manager::getSharedInstance()->orignalVolume;
		auto engine = FMODAudioEngine::sharedEngine();
		engine->m_musicVolume = origVol;
		engine->m_backgroundMusicChannel->setVolume(origVol);
		GameManager::encodeDataTo(p0);
	}
};