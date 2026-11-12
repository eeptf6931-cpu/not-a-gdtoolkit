#include "Calculator.hpp"
namespace gdtoolkit {
FrameWindow frameWindowFor(std::uint64_t n) noexcept {
 if(n<=1) return FrameWindow::One;
 if(n==2) return FrameWindow::Two;
 if(n<=4) return FrameWindow::ThreeToFour;
 if(n<=6) return FrameWindow::FiveToSix;
 if(n<=8) return FrameWindow::SevenToEight;
 if(n<=10) return FrameWindow::NineToTen;
 return FrameWindow::ElevenPlus;
}
}
