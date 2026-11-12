#include "../Runtime.hpp"
#include <algorithm>
#include <Geode/modify/PlayLayer.hpp>
using namespace geode::prelude;
using namespace gdtoolkit_runtime;
class $modify(GDToolkitPlayLayer, PlayLayer) {
    void update(float dt) {
        PlayLayer::update(dt);
        if (!Mod::get()->getSettingValue<bool>("enabled")) {
            g_stats.tracking = false;
            if (g_hud) g_hud->setVisible(false);
            return;
        }

        g_stats.tracking = true;
        ++g_stats.frame;
        ++g_stats.attemptFrames;

        if (m_player1 && m_levelLength > 0.f) {
            g_stats.progress = std::clamp(
                (m_player1->getPositionX() / m_levelLength) * 100.f, 0.f, 100.f
            );
            g_stats.bestProgress = std::max(g_stats.bestProgress, g_stats.progress);
        }

        if (++g_hudRefresh >= 6) {
            g_hudRefresh = 0;
            updateHUD(this);
        }
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;

        ++g_stats.attempts;
        g_stats.bestClicks = std::max(g_stats.bestClicks, g_stats.clicks);
        g_stats.bestFrames = std::max(g_stats.bestFrames, g_stats.attemptFrames);
        g_stats.clicks = 0;
        g_stats.attemptFrames = 0;
        g_stats.progress = 0.f;
        g_stats.lastPressFrame = 0;
        g_stats.hasLastPress = false;
        g_stats.currentWindows.fill(0);
    }

    void levelComplete() {
        PlayLayer::levelComplete();
        g_stats.bestProgress = 100.f;
        g_stats.tracking = false;
    }

    void onQuit() {
        g_stats.tracking = false;
        if (g_hud) {
            g_hud->removeFromParent();
            g_hud = nullptr;
        }
        PlayLayer::onQuit();
    }
};
