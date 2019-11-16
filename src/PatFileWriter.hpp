#ifndef SRC_PATFILEWRITER_HPP_
#define SRC_PATFILEWRITER_HPP_

#include "SessionObject.hpp"

namespace patchscript {

class PatFileWriter {
	std::ostream& out_;
public:
	PatFileWriter(std::ostream& os);
	virtual ~PatFileWriter();
	void write(const SessionObject& po);
};

} /* namespace patchscript */

#endif /* SRC_PATFILEREADER_HPP_ */
