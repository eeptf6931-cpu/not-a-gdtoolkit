#pragma once
#include <cstdint>
namespace gdtoolkit {
enum class FrameWindow : std::uint8_t { One, Two, ThreeToFour, FiveToSix, SevenToEight, NineToTen, ElevenPlus };
FrameWindow frameWindowFor(std::uint64_t delta) noexcept;
}
