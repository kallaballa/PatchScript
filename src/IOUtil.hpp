#ifndef SRC_IOUTIL_HPP_
#define SRC_IOUTIL_HPP_

#include <map>
#include <string>
#include <iostream>

namespace patchscript {
std::map<std::string, std::string> read_key_value(std::istream& in);
std::string read_fully(std::istream& in);
} /* namespace patchscript */

#endif /* SRC_IOUTIL_HPP_ */
