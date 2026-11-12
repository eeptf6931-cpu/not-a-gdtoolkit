#include <Geode/Geode.hpp>
#include "Runtime.hpp"
#include "Common.hpp"
#include "Math/Calculator.hpp"
#include "IO/FileIO.hpp"
#include "UI/SessionReportPopup.hpp"

#include <array>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace geode::prelude;

namespace gdtoolkit_runtime {
    // Frame-window measurements are based on actual game input callbacks,
    // not on estimated FPS or wall-clock time.
    SessionStats g_stats;
    std::vector<InputEvent> g_events;
    CCLabelBMFont* g_hud = nullptr;
    uint32_t g_hudRefresh = 0;

    size_t bucketFor(uint64_t frames) {
        return static_cast<size_t>(gdtoolkit::frameWindowFor(frames));
    }

    void recordInput(bool down, int button, bool isPlayer1, PlayerObject* player) {
        if (!g_stats.tracking) return;

        if (player) {
            if (g_events.size() < gdtoolkit::kMaxRecordedEvents) {
                g_events.push_back({
                    g_stats.attempts, g_stats.frame, button,
                    isPlayer1 ? gdtoolkit::PlayerSlot::Player1 : gdtoolkit::PlayerSlot::Player2,
                    down ? gdtoolkit::InputAction::Press : gdtoolkit::InputAction::Release,
                    player->getPositionX(), player->getPositionY(), g_stats.progress
                });
            }
        }

        if (!down || !isPlayer1 || button != 1) return;

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
            << "\nBest clicks in an attempt: " << g_stats.bestClicks
            << "\nLongest attempt: " << g_stats.bestFrames << " frames"
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
            << "  11+f " << g_stats.windows[6]
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
                 << "  \"best_clicks\": " << g_stats.bestClicks << ",\n"
                 << "  \"longest_attempt_frames\": " << g_stats.bestFrames << ",\n"
                 << "  \"frame_windows\": {\n"
                 << "    \"1\": " << g_stats.windows[0] << ",\n"
                 << "    \"2\": " << g_stats.windows[1] << ",\n"
                 << "    \"3-4\": " << g_stats.windows[2] << ",\n"
                 << "    \"5-6\": " << g_stats.windows[3] << ",\n"
                 << "    \"7-8\": " << g_stats.windows[4] << ",\n"
                 << "    \"9-10\": " << g_stats.windows[5] << ",\n"
                 << "    \"11+\": " << g_stats.windows[6] << "\n"
                 << "  },\n"
                 << "  \"events\": [\n";
            for (size_t i = 0; i < g_events.size(); ++i) {
                const auto& event = g_events[i];
                json << "    {\"attempt\": " << event.attempt
                     << ", \"frame\": " << event.frame
                     << ", \"button\": " << event.button
                     << ", \"player\": " << static_cast<unsigned>(event.player)
                     << ", \"action\": \"" << (event.action == gdtoolkit::InputAction::Press ? "press" : "release")
                     << "\", \"x\": " << event.x
                     << ", \"y\": " << event.y
                     << ", \"progress\": " << event.progressPercent << "}";
                json << (i + 1 == g_events.size() ? "\n" : ",\n");
            }
            json << "  ]\n"
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
                << "best_clicks," << g_stats.bestClicks << "\n"
                << "longest_attempt_frames," << g_stats.bestFrames << "\n"
                << "window_1," << g_stats.windows[0] << "\n"
                << "window_2," << g_stats.windows[1] << "\n"
                << "window_3_4," << g_stats.windows[2] << "\n"
                << "window_5_6," << g_stats.windows[3] << "\n"
                << "window_7_8," << g_stats.windows[4] << "\n"
                << "window_9_10," << g_stats.windows[5] << "\n"
                << "window_11_plus," << g_stats.windows[6] << "\n";
        }
        {
            std::string error;
            if (!gdtoolkit::io::writeEventsCsv(dir / "gd-toolkit-events.csv", g_events, error)) {
                log::warn("Could not export input event CSV: {}", error);
            }
        }
        FLAlertLayer::create("GD Toolkit", "Session summary and input history exported as JSON/CSV in the mod save folder.", "Close")->show();
    }

    void showReport() {
        gdtoolkit_ui::showSessionReport(makeReport());
    }
}


$on_mod(Loaded) {
    log::info("[GD Toolkit] Loaded: live click counter and frame-window tracking ready.");
}

