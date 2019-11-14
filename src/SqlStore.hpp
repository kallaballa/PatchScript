#ifndef PATCHSCRIPT_SRC_SQLSTORE_HPP_
#define PATCHSCRIPT_SRC_SQLSTORE_HPP_

#include <string>
#include <vector>
#include "DB.h"

using std::string;
namespace patchscript {

struct PatchObject {
	string name_;
	int64_t revision_ = -1;
	string runtimeName_;
	string runtimeVersion_;
	string description_;
	string code_;
	int64_t date_ = -1;
	string layout_;
	string parameters_;
	string keyboardBindings_;
	string midiBindings_;

	bool isEmpty() const {
		return name_.empty()
				&& revision_ == -1
				&& runtimeName_.empty()
				&& runtimeVersion_.empty()
				&& description_.empty()
				&& code_.empty()
				&& date_ == -1;
	}
};

class SqlStore {
	sqlite::DB db_;
	sqlite::Statement* stmtCreatePatches_;
	sqlite::Statement* stmtCreateTrash_;
	sqlite::Statement* stmtInsertPatch_;
	sqlite::Statement* stmtSelectAllPatches_;
	sqlite::Statement* stmtSelectPatches_;
	sqlite::Statement* stmtSelectMaxRevision_;
	public:
	SqlStore(const string& dbfile);
	virtual ~SqlStore();
	void storePatch(const PatchObject& po);
	void selectPatches(const PatchObject& po, std::vector<PatchObject>& result);
	void deletePatches(const PatchObject& po);
	void listPatches(std::vector<PatchObject>& result);

};

} /* namespace patchscript */

#endif /* PATCHSCRIPT_SRC_SQLSTORE_HPP_ */
