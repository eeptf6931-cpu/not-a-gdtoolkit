#pragma once
#include <cstdint>
#include <string_view>
namespace gdtoolkit {
inline constexpr std::string_view kModName = "GD Toolkit";
inline constexpr std::uint32_t kMaxRecordedEvents = 100000;
inline constexpr std::uint32_t kMaxMacroBytes = 64u * 1024u * 1024u;
}
