#pragma once
#include <functional>
#include <string_view>
namespace gdtoolkit {
class SoundManager {
public:
 using PlaybackCallback=std::function<void(std::string_view)>;
 void setPlaybackCallback(PlaybackCallback);
 void playCue(std::string_view) const;
private: PlaybackCallback m_callback;
};
}
