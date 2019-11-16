#ifndef SRC_SESSIONOBJECT_HPP_
#define SRC_SESSIONOBJECT_HPP_

#include <string>
#include <iostream>
#include <cstdint>

namespace patchscript {

using std::string;

struct SessionObject {
	string name_;
	string author_;
	int64_t revision_ = -1;
	string runtimeName_;
	string runtimeVersion_;
	string description_;
	string code_;
	int64_t date_ = -1;
	string layout_;
	string parameters_;
	string keyboardBindings_;
	string midiBindings_;

	bool isEmpty() const {
		return name_.empty()
				&& revision_ == -1
				&& runtimeName_.empty()
				&& runtimeVersion_.empty()
				&& description_.empty()
				&& code_.empty()
				&& date_ == -1;
	}
};

std::ostream& operator<<(std::ostream& os, const SessionObject& so);

} /* namespace patchscript */

#endif /* SRC_SESSIONOBJECT_HPP_ */
