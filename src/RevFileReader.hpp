#ifndef SRC_REVFILEREADER_HPP_
#define SRC_REVFILEREADER_HPP_

#include "SqlStore.hpp"
#include <string>

namespace patchscript {

class RevFileReader {
	std::istream& in_;
	std::string name_;
public:
	RevFileReader(const std::string& name, std::istream& is);
	virtual ~RevFileReader();
	bool read(PatchObject& po);
};

} /* namespace patchscript */

#endif /* SRC_PATFILEREADER_HPP_ */
