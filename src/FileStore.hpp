#ifndef SRC_FILESTORE_HPP_
#define SRC_FILESTORE_HPP_

#include "PatchScript.hpp"

#include <string>
#ifdef __APPLE__
#include "filesystem.hpp"
#else
#include <experimental/filesystem>
#endif

namespace patchscript {

using std::string;

class FileStore {
	fs::path fsDir_;
public:
	FileStore(const string& strDirectory);
	virtual ~FileStore();
	void store(std::vector<SessionObject> list);
	void retrieve(std::vector<SessionObject>& list);
};
} /* namespace patchscript */

#endif /* SRC_FILESTORE_HPP_ */
