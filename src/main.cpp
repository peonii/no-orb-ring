#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

static void* orig_spawnCircle = nullptr;

void RingObject_spawnCircle(RingObject* self) {
	if (!Mod::get()->getSettingValue<bool>("disable-orb-ring")) {
		return reinterpret_cast<void(*)(RingObject*)>(orig_spawnCircle)(self);
	}
}

$execute {
	orig_spawnCircle = reinterpret_cast<void*>(base::get() + 0x4896d0);

    Mod::get()->hook(reinterpret_cast<void*>(base::get() + 0x4896d0),
                     &RingObject_spawnCircle, "RingObject::spawnCircle",
                     tulip::hook::TulipConvention::Default);

	// incredible code quality
	auto dashBoomPatchResult = Mod::get()->patch(reinterpret_cast<void*>(base::get() + 0x38002f), { 0x90, 0xe9, 0x14, 0x02, 0x00, 0x00 });

	if (dashBoomPatchResult.isErr()) {
		log::error("Failed to patch PlayerObject::startDashing");
		return;
	}

	auto dashBoomPatch = dashBoomPatchResult.unwrap();

	if (!Mod::get()->getSettingValue<bool>("disable-dash-boom")) {
		dashBoomPatch->disable();
	}

	listenForSettingChanges("disable-dash-boom", [dashBoomPatch](bool value) {
		if (value) {
			dashBoomPatch->enable();
		} else {
			dashBoomPatch->disable();
		}
	});
}