/*
 * PatFile.hpp
 *
 *  Created on: Nov 15, 2019
 *      Author: elchaschab
 */

#ifndef SRC_REVFILEWRITER_HPP_
#define SRC_REVFILEWRITER_HPP_

#include "SqlStore.hpp"

namespace patchscript {

class RevFileWriter {
	std::ostream& out_;
public:
	RevFileWriter(std::ostream& os);
	virtual ~RevFileWriter();
	void write(const PatchObject& po);
};

} /* namespace patchscript */

#endif /* SRC_PATFILEREADER_HPP_ */
