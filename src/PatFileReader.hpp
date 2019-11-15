#ifndef SRC_PATFILEREADER_HPP_
#define SRC_PATFILEREADER_HPP_

#include "SqlStore.hpp"

namespace patchscript {

class PatFileReader {
	std::istream& in_;
public:
	PatFileReader(std::istream& is);
	virtual ~PatFileReader();
	void read(PatchObject& po);
};

} /* namespace patchscript */

#endif /* SRC_PATFILEREADER_HPP_ */
