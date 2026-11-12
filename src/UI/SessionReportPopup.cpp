#include "SessionReportPopup.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;
namespace gdtoolkit_ui {
void showSessionReport(const std::string& report) {
    FLAlertLayer::create("GD Toolkit — Session", report, "Close")->show();
}
}
