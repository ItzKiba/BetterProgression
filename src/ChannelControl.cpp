#include <Geode/fmod/fmod.hpp>
#include "Manager.hpp"

// credits to dankmeme01 for the hooking help

FMOD_RESULT setVolume(FMOD::ChannelControl* channel, float volume) {
    auto manager = Manager::getSharedInstance();
    log::info("volume: {}", volume);
    log::info("manager->volChangeNotFromTierUp: {}", manager->volChangeNotFromTierUp);
	auto result = channel->setVolume(volume);
	if (manager->volChangeNotFromTierUp) {
	    manager->orignalVolume = volume;
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