#include "RevFileReader.hpp"
#include "IOUtil.hpp"
#include <sstream>

namespace patchscript {


RevFileReader::RevFileReader(const string& name, std::istream& is) : in_(is), name_(name) {
}

RevFileReader::~RevFileReader() {
}

bool RevFileReader::read(SessionObject& so) {
	string metaRev;
	if (!getline(in_, metaRev, (char) 0))
		return false;
	string codeRev;

	if (!getline(in_, codeRev, (char) 0))
		return false;
	so.name_ = name_;
	so.code_ = codeRev;

	std::istringstream metaRevIs(metaRev);
	auto metaRevMap = read_key_value(metaRevIs);
	so.author_ = metaRevMap["author"];
	so.revision_ = std::atoll(metaRevMap["revision"].c_str());
	so.runtimeName_ = metaRevMap["runtimeName"];
	so.runtimeVersion_ = metaRevMap["runtimeVersion"];
	so.description_ = metaRevMap["description"];
	so.date_ = std::atoll(metaRevMap["date"].c_str());
	so.layout_ = metaRevMap["layout"];
	so.parameters_ = metaRevMap["parameters"];
	so.keyboardBindings_ = metaRevMap["keyboardBindings"];
	so.midiBindings_ = metaRevMap["midiBindings"];
	return in_.good();
}
} /* namespace patchscript */
