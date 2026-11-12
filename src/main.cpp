#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

namespace {
    unsigned long long g_totalFrames = 0;
    unsigned int g_framesSinceLog = 0;
}

class $modify(GDToolkitPlayLayer, PlayLayer) {
    void update(float dt) {
        PlayLayer::update(dt);

        ++g_totalFrames;
        ++g_framesSinceLog;

        const bool enabled = Mod::get()->getSettingValue<bool>("show-frame-log");
        const int interval = Mod::get()->getSettingValue<int>("log-interval");

        if (enabled && interval > 0 &&
            g_framesSinceLog >= static_cast<unsigned int>(interval)) {
            log::info(
                "[GD Toolkit] PlayLayer update count: {} (session total)",
                g_totalFrames
            );
            g_framesSinceLog = 0;
        }
    }
};

$execute {
    log::info("[GD Toolkit] Loaded. Initial frame diagnostics are active.");
}
