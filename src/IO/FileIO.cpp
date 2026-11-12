#include "FileIO.hpp"
#include <fstream>
namespace gdtoolkit::io {
bool writeEventsCsv(const std::filesystem::path& path,const std::vector<InputEvent>& events,std::string& error) {
 std::ofstream out(path,std::ios::binary);
 if(!out){error="Unable to open output file";return false;}
 out<<"attempt,frame,button,player,action,x,y,progress_percent\n";
 for(const auto& e:events) out<<e.attempt<<','<<e.frame<<','<<e.button<<','<<static_cast<unsigned>(e.player)<<','<<(e.action==InputAction::Press?"press":"release")<<','<<e.x<<','<<e.y<<','<<e.progressPercent<<'\n';
 if(!out.good()){error="Failed while writing CSV data";return false;}
 error.clear(); return true;
}
}
