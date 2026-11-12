#pragma once
#include <Geode/Geode.hpp>
#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace gdtoolkit_runtime {
struct InputEvent {
    std::uint64_t attempt = 1;
    std::uint64_t frame = 0;
    int button = 0;
    int player = 1;
    bool down = false;
    float x = 0.f;
    float y = 0.f;
    float progress = 0.f;
};
struct SessionStats {
    std::uint64_t frame = 0;
    std::uint64_t attemptFrames = 0;
    std::uint64_t attempts = 1;
    std::uint64_t clicks = 0;
    std::uint64_t bestClicks = 0;
    std::uint64_t bestFrames = 0;
    std::uint64_t lastPressFrame = 0;
    bool hasLastPress = false;
    std::array<std::uint64_t, 7> windows{};
    std::array<std::uint64_t, 7> currentWindows{};
    float bestProgress = 0.f;
    float progress = 0.f;
    bool tracking = false;
};
extern SessionStats g_stats;
extern std::vector<InputEvent> g_events;
extern geode::prelude::CCLabelBMFont* g_hud;
extern std::uint32_t g_hudRefresh;
std::size_t bucketFor(std::uint64_t frames);
void recordInput(bool down, int button, bool isPlayer1, geode::prelude::PlayerObject* player);
void updateHUD(geode::prelude::PlayLayer* layer);
void exportSession();
void showReport();
}
