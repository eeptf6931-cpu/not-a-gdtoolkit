#pragma once
#include "../Data/Types.hpp"
#include <filesystem>
#include <string>
#include <vector>
namespace gdtoolkit::io {
bool writeEventsCsv(const std::filesystem::path&, const std::vector<InputEvent>&, std::string& error);
}
