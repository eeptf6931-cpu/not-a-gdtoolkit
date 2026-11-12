#include "SoundManager.hpp"
#include <utility>
namespace gdtoolkit {
void SoundManager::setPlaybackCallback(PlaybackCallback cb){m_callback=std::move(cb);}
void SoundManager::playCue(std::string_view cue) const {if(m_callback)m_callback(cue);}
}
