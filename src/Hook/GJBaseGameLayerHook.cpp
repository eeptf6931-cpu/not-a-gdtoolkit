#include "../Runtime.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>
using namespace geode::prelude;
using namespace gdtoolkit_runtime;
class $modify(GDToolkitGameLayer, GJBaseGameLayer) {
    void handleButton(bool down, int button, bool isPlayer1) {
        GJBaseGameLayer::handleButton(down, button, isPlayer1);
        if (Mod::get()->getSettingValue<bool>("enabled")) {
            recordInput(down, button, isPlayer1, isPlayer1 ? this->m_player1 : this->m_player2);
        }
    }
};
