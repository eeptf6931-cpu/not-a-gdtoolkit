#include "State.hpp"
#include "../Math/Calculator.hpp"
#include <algorithm>
namespace gdtoolkit {
void SessionState::tick() noexcept { ++frame; ++attemptFrames; }
void SessionState::beginAttempt() noexcept {
 bestClicks=std::max(bestClicks,clicks);
 bestAttemptFrames=std::max(bestAttemptFrames,attemptFrames);
 ++attempts; clicks=0; attemptFrames=0; progressPercent=0.f;
 hasLastPress=false; lastPressFrame=0;
}
void SessionState::recordPress(std::uint64_t now) noexcept {
 ++clicks;
 if(hasLastPress && now>lastPressFrame) {
  switch(frameWindowFor(now-lastPressFrame)) {
   case FrameWindow::One: ++windows.one; break;
   case FrameWindow::Two: ++windows.two; break;
   case FrameWindow::ThreeToFour: ++windows.threeToFour; break;
   case FrameWindow::FiveToSix: ++windows.fiveToSix; break;
   case FrameWindow::SevenToEight: ++windows.sevenToEight; break;
   case FrameWindow::NineToTen: ++windows.nineToTen; break;
   case FrameWindow::ElevenPlus: ++windows.elevenPlus; break;
  }
 }
 lastPressFrame=now; hasLastPress=true;
}
}
