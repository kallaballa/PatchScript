#include "SessionObject.hpp"

namespace patchscript {

std::ostream& operator<<(std::ostream& os, const SessionObject& so) {
	os << "name=" << so.name_ << '\n';
	os << "author=" << so.author_ << '\n';
	os << "revision=" << so.revision_ << '\n';
	os << "runtimeName=" << so.runtimeName_ << '\n';
	os << "runtimeVersion=" << so.runtimeVersion_ << '\n';
	os << "description=" << so.description_ << '\n';
	os << "date=" << so.date_ << '\n';
	os << "layout=" << so.layout_ << '\n';
	os << "parameters=" << so.parameters_ << '\n';
	os << "keyboardBindings=" << so.keyboardBindings_ << '\n';
	os << "midiBindings=" << so.midiBindings_ << std::endl;
	return os;
}

} /* namespace patchscript */
