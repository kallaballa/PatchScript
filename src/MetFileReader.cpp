#include "MetFileReader.hpp"
#include "IOUtil.hpp"

namespace patchscript {

MetFileReader::MetFileReader(std::istream& is) : in_(is) {
}

MetFileReader::~MetFileReader() {
}

void MetFileReader::read(SessionObject& so) {
	auto metaMap = read_key_value(in_);
	so.author_ = std::atoll(metaMap["author"].c_str());
	so.revision_ = std::atoll(metaMap["revision"].c_str());
	so.runtimeName_ = metaMap["runtimeName"];
	so.runtimeVersion_ = metaMap["runtimeVersion"];
	so.description_= metaMap["description"];
	so.date_ = std::atoll(metaMap["date"].c_str());
	so.layout_ = metaMap["layout"];
	so.parameters_ = metaMap["parameters"];
	so.keyboardBindings_ = metaMap["keyboardBindings"];
	so.midiBindings_= metaMap["midiBindings"];
}

} /* namespace patchscript */
