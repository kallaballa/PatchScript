#ifndef SRC_METFILEWRITER_HPP_
#define SRC_METFILEWRITER_HPP_

#include "SqlStore.hpp"

namespace patchscript {

class MetFileWriter {
	std::ostream& out_;
public:
	MetFileWriter(std::ostream& os);
	virtual ~MetFileWriter();
	void write(const PatchObject& po);
};

} /* namespace patchscript */

#endif /* SRC_PATFILEREADER_HPP_ */
