#include "IOUtil.hpp"
#include <sstream>

namespace patchscript {
std::map<std::string, std::string> read_key_value(std::istream& in) {
	std::map<std::string, std::string> kv;
	std::string line;
	while (std::getline(in, line)) {
		std::istringstream is_line(line);
		std::string key;
		if (std::getline(is_line, key, '=')) {
			std::string value;
			if (std::getline(is_line, value)) {
				kv[key] = value;
			}
		}
	}
	return kv;
}

std::string read_fully(std::istream& in) {
    std::stringstream sstr;
    sstr << in.rdbuf();
     return sstr.str();
}

} /* namespace patchscript */
