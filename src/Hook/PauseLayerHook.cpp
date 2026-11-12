#include "../Runtime.hpp"
#include <Geode/modify/PauseLayer.hpp>
using namespace geode::prelude;
using namespace gdtoolkit_runtime;
class $modify(GDToolkitPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;

        auto winSize = CCDirector::get()->getWinSize();
        auto menu = CCMenu::create();
        if (!menu) return;
        menu->setPosition({0.f, 0.f});
        this->addChild(menu, 100);

        auto sprite = ButtonSprite::create("Toolkit", "bigFont.fnt", "GJ_button_01.png", 0.7f);
        if (!sprite) return;
        auto button = CCMenuItemSpriteExtra::create(
            sprite, this, menu_selector(GDToolkitPauseLayer::onToolkit)
        );
        if (!button) return;
        button->setPosition({winSize.width - 48.f, winSize.height - 34.f});
        menu->addChild(button);

        auto exportSprite = ButtonSprite::create("Export", "bigFont.fnt", "GJ_button_01.png", 0.7f);
        if (exportSprite) {
            auto exportButton = CCMenuItemSpriteExtra::create(
                exportSprite, this, menu_selector(GDToolkitPauseLayer::onExport)
            );
            if (exportButton) {
                exportButton->setPosition({winSize.width - 48.f, winSize.height - 76.f});
                menu->addChild(exportButton);
            }
        }
    }

    void onExport(CCObject*) {
        exportSession();
    }

    void onToolkit(CCObject*) {
        showReport();
        if (Mod::get()->getSettingValue<bool>("open-settings-from-pause")) {
            geode::openSettingsPopup(Mod::get());
        }
    }
};
