/*
 * GitStore.cpp
 *
 *  Created on: Nov 11, 2019
 *      Author: elchaschab
 */

#include "SqlStore.hpp"
#include "Exception.h"
#include "fts3_tokenizer.h"
#include "sqlite3.h"
#include "Statement.h"
#include "Tokenizer.h"
#include <sstream>
#include <iostream>

namespace patchscript {

const string CREATE_PATCHES = R"_STATEMENT_(
		CREATE TABLE IF NOT EXISTS patches (
	    name TEXT NOT NULL,
	    revision INTEGER NOT NULL,
	    runtimeName TEXT NOT NULL,
	    runtimeVersion TEXT NOT NULL,
	    description TEXT NOT NULL,
	    code TEXT NOT NULL,
			date INTEGER NOT NULL,
			PRIMARY KEY(name,revision)
		);
		)_STATEMENT_";

const string INSERT_PATCH = "INSERT INTO patches VALUES(?, ?, ?, ?, ?, ?, ?);";
const string SELECT_ALL_PATCHES = "SELECT * FROM patches ORDER BY name, date;";
const string SELECT_PATCHES = "SELECT * FROM patches ";
const string SELECT_MAX_REVISION = "SELECT max(revision) FROM patches WHERE name == ?;";

SqlStore::SqlStore(const string& dbfile) : db_(dbfile) {
	//create patches table if the db is empty
	db_.update(CREATE_PATCHES);
}

SqlStore::~SqlStore() {
	db_.close();
}

void SqlStore::store(const PatchObject& po) {
	db_.update("BEGIN TRANSACTION;");
	auto stmt = db_.query(SELECT_MAX_REVISION, po.name_);

	if(stmt.step() == sqlite::Statement::ROW) {
		db_.update(INSERT_PATCH, po.name_, stmt.column_int64(0) + 1, po.runtimeName_, po.runtimeVersion_, po.description_, po.code_, po.date_);
	} else {
		db_.update(INSERT_PATCH, po.name_, 0, po.runtimeName_, po.runtimeVersion_, po.description_, po.code_, po.date_);
	}

	db_.update("COMMIT;");
}

void SqlStore::select(const PatchObject& po, std::vector<PatchObject>& result) {
	std::ostringstream ss;
	ss << SELECT_PATCHES;
	std::vector<string> whereClauses;
	if(!po.isEmpty()) {
		ss << "WHERE ";
		if(!po.name_.empty())
			whereClauses.push_back(string("name LIKE '%") + po.name_ + "%'");
		if(po.revision_ != -1)
			whereClauses.push_back(string("revision == ") + std::to_string(po.revision_));
		if(!po.runtimeName_.empty())
			whereClauses.push_back(string("runtimeName LIKE '%") + po.runtimeName_ + "%'");
		if(!po.runtimeVersion_.empty())
			whereClauses.push_back(string("runtimeVersion LIKE '%") + po.runtimeVersion_ + "%'");
		if(!po.description_.empty())
			whereClauses.push_back(string("description LIKE '%") + po.description_ + "%'");
		if(!po.code_.empty())
			whereClauses.push_back(string("code LIKE '%") + po.code_ + "%'");
		if(po.date_ != -1)
			whereClauses.push_back(string("date > ") + std::to_string(po.date_));

		for(size_t i = 0; i < whereClauses.size(); ++i) {
			ss << whereClauses[i];
			if(i < whereClauses.size() - 1)
				ss << " AND ";
		}
	}

	ss << " GROUP BY name ORDER BY date;";

	auto stmt = db_.query(ss.str());

	while (stmt.step() == sqlite::Statement::ROW) {
		result.push_back(PatchObject{stmt.column_string(0), stmt.column_int64(1), stmt.column_string(2), stmt.column_string(3), stmt.column_string(4), stmt.column_string(5), stmt.column_int64(6)});
	}
}

void SqlStore::list(std::vector<PatchObject>& result) {
	auto stmt = db_.query(SELECT_ALL_PATCHES);

	while (stmt.step() == sqlite::Statement::ROW) {
		result.push_back(PatchObject{stmt.column_string(0), stmt.column_int64(1), stmt.column_string(2), stmt.column_string(3), stmt.column_string(4), stmt.column_string(5), stmt.column_int64(6)});
	}
}
} /* namespace patchscript */
