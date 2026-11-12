#pragma once
#include <cstdint>
namespace gdtoolkit {
enum class InputAction : std::uint8_t { Press, Release };
enum class PlayerSlot : std::uint8_t { Player1 = 1, Player2 = 2 };
struct InputEvent {
 std::uint64_t attempt=0, frame=0;
 std::int32_t button=0;
 PlayerSlot player=PlayerSlot::Player1;
 InputAction action=InputAction::Press;
 float x=0.f, y=0.f, progressPercent=0.f;
};
struct FrameWindowCounts {
 std::uint64_t one=0, two=0, threeToFour=0, fiveToSix=0;
 std::uint64_t sevenToEight=0, nineToTen=0, elevenPlus=0;
};
}
