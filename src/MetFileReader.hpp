#ifndef SRC_METFILEREADER_HPP_
#define SRC_METFILEREADER_HPP_

#include "SqlStore.hpp"

namespace patchscript {

class MetFileReader {
	std::istream& in_;
public:
	MetFileReader(std::istream& is);
	virtual ~MetFileReader();
	void read(PatchObject& po);
};

} /* namespace patchscript */

#endif /* SRC_PATFILEREADER_HPP_ */
