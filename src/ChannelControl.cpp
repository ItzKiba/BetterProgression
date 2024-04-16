#include <Geode/fmod/fmod.hpp>
#include "Manager.hpp"

// credits to dankmeme01 for the hooking help

FMOD_RESULT setVolume(FMOD::ChannelControl* channel, float volume) {
    auto manager = Manager::getSharedInstance();
	auto result = channel->setVolume(volume);
	if (manager->volChangeNotFromTierUp && channel == FMODAudioEngine::sharedEngine()->m_backgroundMusicChannel) {
	    manager->orignalVolume = volume;
	    log::info("volume: {}", volume);
	}
	return result;
}

$execute {
    (void) Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getNonVirtual(
                &FMOD::ChannelControl::setVolume
            )
        ),
        &setVolume,
        "FMOD::ChannelControl::setVolume",
        tulip::hook::TulipConvention::Stdcall
    ).expect("Failed to hook FMOD.").unwrap();
}