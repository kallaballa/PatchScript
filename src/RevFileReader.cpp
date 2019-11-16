#include "RevFileReader.hpp"
#include "IOUtil.hpp"
#include <sstream>

namespace patchscript {


RevFileReader::RevFileReader(const string& name, std::istream& is) : in_(is), name_(name) {
}

RevFileReader::~RevFileReader() {
}

bool RevFileReader::read(SessionObject& po) {
	string metaRev;
	if (!getline(in_, metaRev, (char) 0))
		return false;
	string codeRev;

	if (!getline(in_, codeRev, (char) 0))
		return false;
	po.name_ = name_;
	po.code_ = codeRev;

	std::istringstream metaRevIs(metaRev);
	auto metaRevMap = read_key_value(metaRevIs);
	po.revision_ = std::atoll(metaRevMap["revision"].c_str());
	po.runtimeName_ = metaRevMap["runtimeName"];
	po.runtimeVersion_ = metaRevMap["runtimeVersion"];
	po.description_ = metaRevMap["description"];
	po.date_ = std::atoll(metaRevMap["date"].c_str());
	po.layout_ = metaRevMap["layout"];
	po.parameters_ = metaRevMap["parameters"];
	po.keyboardBindings_ = metaRevMap["keyboardBindings"];
	po.midiBindings_ = metaRevMap["midiBindings"];
	return in_.good();
}
} /* namespace patchscript */
