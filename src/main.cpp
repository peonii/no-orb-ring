#include <Geode/Geode.hpp>

using namespace geode::prelude;

static void* orig_spawnCircle = nullptr;

#ifdef GEODE_IS_WINDOWS
void RingObject_spawnCircle(RingObject* self) {
	if (!Mod::get()->getSettingValue<bool>("is-enabled")) {
		return reinterpret_cast<void(*)(RingObject*)>(orig_spawnCircle)(self);
	}
}

$execute {
	orig_spawnCircle = reinterpret_cast<void*>(geode::base::get() + 0x4896d0);

    Mod::get()->hook(reinterpret_cast<void*>(geode::base::get() + 0x4896d0),
                     &RingObject_spawnCircle, "RingObject::spawnCircle",
                     tulip::hook::TulipConvention::Default);
}
#endif