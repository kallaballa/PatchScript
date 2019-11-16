#include "SessionObject.hpp"

namespace patchscript {

std::ostream& operator<<(std::ostream& os, const SessionObject& po) {
	os << "name=" << po.name_ << '\n';
	os << "revision=" << po.revision_ << '\n';
	os << "runtimeName=" << po.runtimeName_ << '\n';
	os << "runtimeVersion=" << po.runtimeVersion_ << '\n';
	os << "description=" << po.description_ << '\n';
	os << "date=" << po.date_ << '\n';
	os << "layout=" << po.layout_ << '\n';
	os << "parameters=" << po.parameters_ << '\n';
	os << "keyboardBindings=" << po.keyboardBindings_ << '\n';
	os << "midiBindings=" << po.midiBindings_ << std::endl;
	return os;
}

} /* namespace patchscript */
