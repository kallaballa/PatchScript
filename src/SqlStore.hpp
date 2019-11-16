#ifndef PATCHSCRIPT_SRC_SQLSTORE_HPP_
#define PATCHSCRIPT_SRC_SQLSTORE_HPP_

#include <string>
#include <vector>
#include <iostream>
#include "DB.h"
#include "SessionObject.hpp"

using std::string;
namespace patchscript {

class SqlStore {
	sqlite::DB db_;
	sqlite::Statement* stmtCreateSessions_;
	sqlite::Statement* stmtCreateTrash_;
	sqlite::Statement* stmtInsertSession_;
	sqlite::Statement* stmtSelectAllSessions_;
	sqlite::Statement* stmtSelectSessions_;
	sqlite::Statement* stmtSelectMaxRevision_;
	public:
	SqlStore(const string& dbfile);
	virtual ~SqlStore();
	void store(const SessionObject& so);
	void select(const SessionObject& so, std::vector<SessionObject>& result);
	void remove(const SessionObject& so);
	void list(std::vector<SessionObject>& result);

};

} /* namespace patchscript */

#endif /* PATCHSCRIPT_SRC_SQLSTORE_HPP_ */
