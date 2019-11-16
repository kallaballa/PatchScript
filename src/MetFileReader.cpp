#include "MetFileReader.hpp"
#include "IOUtil.hpp"

namespace patchscript {

MetFileReader::MetFileReader(std::istream& is) : in_(is) {
}

MetFileReader::~MetFileReader() {
}

void MetFileReader::read(SessionObject& po) {
	auto metaMap = read_key_value(in_);
	po.revision_ = std::atoll(metaMap["revision"].c_str());
	po.runtimeName_ = metaMap["runtimeName"];
	po.runtimeVersion_ = metaMap["runtimeVersion"];
	po.description_= metaMap["description"];
	po.date_ = std::atoll(metaMap["date"].c_str());
	po.layout_ = metaMap["layout"];
	po.parameters_ = metaMap["parameters"];
	po.keyboardBindings_ = metaMap["keyboardBindings"];
	po.midiBindings_= metaMap["midiBindings"];
}

} /* namespace patchscript */
