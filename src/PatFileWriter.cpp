/*
 * PatFile.cpp
 *
 *  Created on: Nov 15, 2019
 *      Author: elchaschab
 */

#include "PatFileWriter.hpp"

#include <iostream>

namespace patchscript {

PatFileWriter::PatFileWriter(std::ostream& os) : out_(os) {
}

PatFileWriter::~PatFileWriter() {
}

void PatFileWriter::write(const PatchObject& po) {
	out_ << po.code_;
}

} /* namespace patchscript */
