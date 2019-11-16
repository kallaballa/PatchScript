#include "PatFileReader.hpp"
#include "IOUtil.hpp"

namespace patchscript {

PatFileReader::PatFileReader(std::istream& is) : in_(is) {
}

PatFileReader::~PatFileReader() {
}

void PatFileReader::read(SessionObject& po) {
	po.code_ = read_fully(in_);
}

} /* namespace patchscript */
