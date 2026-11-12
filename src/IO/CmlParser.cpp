#include "CmlParser.hpp"
namespace gdtoolkit::io {
CmlParseResult inspectCml(std::span<const std::uint8_t> bytes) {
 if(bytes.empty()) return {false,0,"Empty CML file"};
 if(bytes.size()>64u*1024u*1024u) return {false,0,"CML file exceeds 64 MiB safety limit"};
 return {false,0,"CML payload decoder is not implemented; import was safely rejected"};
}
}
