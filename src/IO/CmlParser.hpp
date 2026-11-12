#pragma once
#include <cstdint>
#include <span>
#include <string>
namespace gdtoolkit::io {
struct CmlParseResult { bool success=false; std::uint8_t version=0; std::string error; };
CmlParseResult inspectCml(std::span<const std::uint8_t> bytes);
}
