#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>

#include <array>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>

using namespace geode::prelude;

namespace {
    // Frame-window measurements are based on actual game input callbacks,
    // not on estimated FPS or wall-clock time.
    struct SessionStats {
        uint64_t frame = 0;
        uint64_t attemptFrames = 0;
        uint64_t attempts = 1;
        uint64_t clicks = 0;
        uint64_t bestClicks = 0;
        uint64_t bestFrames = 0;
        uint64_t lastPressFrame = 0;
        bool hasLastPress = false;
        std::array<uint64_t, 7> windows{}; // 1, 2, 3-4, 5-6, 7-8, 9-10, 11+
        std::array<uint64_t, 7> currentWindows{};
        float bestProgress = 0.f;
        float progress = 0.f;
        bool tracking = false;
    };

    SessionStats g_stats;
    CCLabelBMFont* g_hud = nullptr;
    uint32_t g_hudRefresh = 0;

    size_t bucketFor(uint64_t frames) {
        if (frames <= 1) return 0;
        if (frames == 2) return 1;
        if (frames <= 4) return 2;
        if (frames <= 6) return 3;
        if (frames <= 8) return 4;
        if (frames <= 10) return 5;
        return 6;
    }

    void recordInput(bool down, int button, bool isPlayer1) {
        if (!g_stats.tracking || !down || !isPlayer1 || button != 1) return;

        ++g_stats.clicks;
        if (g_stats.hasLastPress) {
            auto delta = g_stats.frame - g_stats.lastPressFrame;
            if (delta > 0) {
                auto bucket = bucketFor(delta);
                ++g_stats.windows[bucket];
                ++g_stats.currentWindows[bucket];
            }
        }
        g_stats.lastPressFrame = g_stats.frame;
        g_stats.hasLastPress = true;
    }

    std::string makeReport() {
        std::ostringstream out;
        out << "Clicks this attempt: " << g_stats.clicks
            << "\nAttempts this session: " << g_stats.attempts
            << "\nAttempt frames: " << g_stats.attemptFrames
            << "\nCurrent progress: " << std::fixed << std::setprecision(1)
            << g_stats.progress << "%"
            << "\nBest progress: " << g_stats.bestProgress << "%"
            << "\n\nFrame windows (between consecutive jump presses)"
            << "\n1 frame: " << g_stats.windows[0]
            << "\n2 frames: " << g_stats.windows[1]
            << "\n3-4 frames: " << g_stats.windows[2]
            << "\n5-6 frames: " << g_stats.windows[3]
            << "\n7-8 frames: " << g_stats.windows[4]
            << "\n9-10 frames: " << g_stats.windows[5]
            << "\n11+ frames: " << g_stats.windows[6]
            << "\n\nMeasurements are game-update frame counts.";
        return out.str();
    }

    void updateHUD(PlayLayer* layer) {
        if (!Mod::get()->getSettingValue<bool>("show-hud")) {
            if (g_hud) g_hud->setVisible(false);
            return;
        }

        if (!g_hud) {
            g_hud = CCLabelBMFont::create("", "bigFont.fnt");
            if (!g_hud) return;
            g_hud->setAnchorPoint({0.f, 1.f});
            g_hud->setScale((Mod::get()->getSettingValue<int>("hud-scale") / 100.f));
            g_hud->setID("gd-toolkit-hud");
            auto parent = layer->m_uiLayer ? layer->m_uiLayer : static_cast<CCNode*>(layer);
            parent->addChild(g_hud, 9999);
        }
        g_hud->setVisible(true);

        std::ostringstream out;
        out << "GD TOOLKIT"
            << "\nClicks: " << g_stats.clicks
            << "   Attempts: " << g_stats.attempts
            << "\n1f " << g_stats.windows[0]
            << "  2f " << g_stats.windows[1]
            << "  3-4f " << g_stats.windows[2]
            << "\n5-6f " << g_stats.windows[3]
            << "  7-8f " << g_stats.windows[4]
            << "  9-10f " << g_stats.windows[5]
            << "\nBest: " << std::fixed << std::setprecision(1)
            << g_stats.bestProgress << "%";
        g_hud->setString(out.str().c_str());

        auto size = CCDirector::get()->getWinSize();
        g_hud->setPosition({8.f, size.height - 8.f});
        g_hud->setScale((Mod::get()->getSettingValue<int>("hud-scale") / 100.f));
    }

    void exportSession() {
        auto dir = Mod::get()->getSaveDir();
        std::error_code ec;
        std::filesystem::create_directories(dir, ec);
        if (ec) {
            FLAlertLayer::create("GD Toolkit", "Could not create the mod save folder.", "Close")->show();
            return;
        }

        {
            std::ofstream json(dir / "gd-toolkit-session.json", std::ios::binary);
            if (!json) {
                FLAlertLayer::create("GD Toolkit", "Could not write the JSON report.", "Close")->show();
                return;
            }
            json << "{\n"
                 << "  \"attempts\": " << g_stats.attempts << ",\n"
                 << "  \"clicks\": " << g_stats.clicks << ",\n"
                 << "  \"attempt_frames\": " << g_stats.attemptFrames << ",\n"
                 << "  \"progress_percent\": " << std::fixed << std::setprecision(2) << g_stats.progress << ",\n"
                 << "  \"best_progress_percent\": " << g_stats.bestProgress << ",\n"
                 << "  \"frame_windows\": {\n"
                 << "    \"1\": " << g_stats.windows[0] << ",\n"
                 << "    \"2\": " << g_stats.windows[1] << ",\n"
                 << "    \"3-4\": " << g_stats.windows[2] << ",\n"
                 << "    \"5-6\": " << g_stats.windows[3] << ",\n"
                 << "    \"7-8\": " << g_stats.windows[4] << ",\n"
                 << "    \"9-10\": " << g_stats.windows[5] << ",\n"
                 << "    \"11+\": " << g_stats.windows[6] << "\n"
                 << "  }\n"
                 << "}\n";
        }
        {
            std::ofstream csv(dir / "gd-toolkit-session.csv", std::ios::binary);
            if (!csv) {
                FLAlertLayer::create("GD Toolkit", "JSON was saved, but the CSV report could not be written.", "Close")->show();
                return;
            }
            csv << "metric,value\n"
                << "attempts," << g_stats.attempts << "\n"
                << "clicks," << g_stats.clicks << "\n"
                << "attempt_frames," << g_stats.attemptFrames << "\n"
                << "progress_percent," << g_stats.progress << "\n"
                << "best_progress_percent," << g_stats.bestProgress << "\n"
                << "window_1," << g_stats.windows[0] << "\n"
                << "window_2," << g_stats.windows[1] << "\n"
                << "window_3_4," << g_stats.windows[2] << "\n"
                << "window_5_6," << g_stats.windows[3] << "\n"
                << "window_7_8," << g_stats.windows[4] << "\n"
                << "window_9_10," << g_stats.windows[5] << "\n"
                << "window_11_plus," << g_stats.windows[6] << "\n";
        }
        FLAlertLayer::create("GD Toolkit", "Session reports saved as gd-toolkit-session.json and gd-toolkit-session.csv in the mod save folder.", "Close")->show();
    }

    void showReport() {
        FLAlertLayer::create("GD Toolkit — Session", makeReport(), "Close")->show();
    }
}

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

class $modify(GDToolkitGameLayer, GJBaseGameLayer) {
    void handleButton(bool down, int button, bool isPlayer1) {
        GJBaseGameLayer::handleButton(down, button, isPlayer1);
        if (Mod::get()->getSettingValue<bool>("enabled")) {
            recordInput(down, button, isPlayer1);
        }
    }
};

class $modify(GDToolkitPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;

        auto winSize = CCDirector::get()->getWinSize();
        auto menu = CCMenu::create();
        if (!menu) return;
        menu->setPosition({0.f, 0.f});
        this->addChild(menu, 100);

        auto sprite = ButtonSprite::create("Toolkit", "bigFont.fnt", "GJ_button_01.png", 0.7f);
        if (!sprite) return;
        auto button = CCMenuItemSpriteExtra::create(
            sprite, this, menu_selector(GDToolkitPauseLayer::onToolkit)
        );
        if (!button) return;
        button->setPosition({winSize.width - 48.f, winSize.height - 34.f});
        menu->addChild(button);

        auto exportSprite = ButtonSprite::create("Export", "bigFont.fnt", "GJ_button_01.png", 0.7f);
        if (exportSprite) {
            auto exportButton = CCMenuItemSpriteExtra::create(
                exportSprite, this, menu_selector(GDToolkitPauseLayer::onExport)
            );
            if (exportButton) {
                exportButton->setPosition({winSize.width - 48.f, winSize.height - 76.f});
                menu->addChild(exportButton);
            }
        }
    }

    void onExport(CCObject*) {
        exportSession();
    }

    void onToolkit(CCObject*) {
        showReport();
        if (Mod::get()->getSettingValue<bool>("open-settings-from-pause")) {
            geode::openSettingsPopup(Mod::get());
        }
    }
};

$on_mod(Loaded) {
    log::info("[GD Toolkit] Loaded: live click counter and frame-window tracking ready.");
}
