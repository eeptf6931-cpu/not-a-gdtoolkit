#pragma once
#include "Types.hpp"
namespace gdtoolkit {
struct SessionState {
 std::uint64_t frame=0, attemptFrames=0, attempts=1, clicks=0;
 std::uint64_t bestClicks=0, bestAttemptFrames=0, lastPressFrame=0;
 bool hasLastPress=false, tracking=false;
 float progressPercent=0.f, bestProgressPercent=0.f;
 FrameWindowCounts windows{};
 void tick() noexcept;
 void beginAttempt() noexcept;
 void recordPress(std::uint64_t currentFrame) noexcept;
};
}
