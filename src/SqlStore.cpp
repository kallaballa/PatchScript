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
			layout TEXT NULL,
			parameters TEXT NULL,
			keyboardBindings TEXT NULL,
			midiBindings TEXT NULL,
			PRIMARY KEY(name,revision)
		);
		)_STATEMENT_";

const string CREATE_TRASH = R"_STATEMENT_(
		CREATE TABLE IF NOT EXISTS trash (
	    name TEXT NOT NULL,
	    revision INTEGER NOT NULL,
	    runtimeName TEXT NOT NULL,
	    runtimeVersion TEXT NOT NULL,
	    description TEXT NOT NULL,
	    code TEXT NOT NULL,
			date INTEGER NOT NULL,
			layout TEXT NULL,
			parameters TEXT NULL,
			keyboardBindings TEXT NULL,
			midiBindings TEXT NULL
		);
		)_STATEMENT_";

const string INSERT_PATCH = "INSERT INTO patches VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
const string INSERT_TRASH = "INSERT INTO trash ";
const string SELECT_ALL_PATCHES = "SELECT * FROM patches ORDER BY name, date;";
const string SELECT_PATCHES = "SELECT * FROM patches ";
const string SELECT_MAX_REVISION = "SELECT max(revision) FROM patches WHERE name == ?;";
const string DELETE_PATCHES = "DELETE FROM patches ";

std::ostream& operator<<(std::ostream& os, const PatchObject& po) {
	os << "name=" << po.name_ << '\n';
	os << "revision=" << po.revision_ << '\n';
	os << "runtimeName=" << po.runtimeName_ << '\n';
	os << "runtimeVersion=" << po.runtimeVersion_ << '\n';
	os << "description=" << po.description_ << '\n';
	os << "date=" << po.date_ << '\n';
	os << "layout=" << po.layout_ << '\n';
	os << "parameters=" << po.parameters_ << '\n';
	os << "keyboardBindings=" << po.keyboardBindings_ << '\n';
	os << "midiBindings=" << po.midiBindings_ << std::endl;
	return os;
}

SqlStore::SqlStore(const string& dbfile) : db_(dbfile) {
	stmtCreatePatches_ = new sqlite::Statement(db_._db, CREATE_PATCHES);
	stmtCreateTrash_ = new sqlite::Statement(db_._db, CREATE_TRASH);

	//create patches and trash table if the db is empty
	stmtCreatePatches_->step();
	stmtCreateTrash_->step();

	stmtInsertPatch_ = new sqlite::Statement(db_._db, INSERT_PATCH);
	stmtSelectAllPatches_ = new sqlite::Statement(db_._db, SELECT_ALL_PATCHES);
	stmtSelectPatches_ = new sqlite::Statement(db_._db, SELECT_PATCHES);
	stmtSelectMaxRevision_ = new sqlite::Statement(db_._db, SELECT_MAX_REVISION);
}

SqlStore::~SqlStore() {
	delete(stmtCreatePatches_);
	delete(stmtCreateTrash_);
	delete(stmtInsertPatch_);
	delete(stmtSelectAllPatches_);
	delete(stmtSelectPatches_);
	delete(stmtSelectMaxRevision_);
	db_.close();
}

void SqlStore::storePatch(const PatchObject& po) {
	db_.update("BEGIN TRANSACTION;");
	try {
		stmtSelectMaxRevision_->reset();
		stmtSelectMaxRevision_->bind(1, po.name_);

		int64_t revision = 0;
		if (stmtSelectMaxRevision_->step() == sqlite::Statement::ROW) {
			revision = stmtSelectMaxRevision_->column_int64(0) + 1;
		}
		stmtInsertPatch_->reset();
		stmtInsertPatch_->bind(1, po.name_);
		stmtInsertPatch_->bind(2, revision);
		stmtInsertPatch_->bind(3, po.runtimeName_);
		stmtInsertPatch_->bind(4, po.runtimeVersion_);
		stmtInsertPatch_->bind(5, po.description_);
		stmtInsertPatch_->bind(6, po.code_);
		stmtInsertPatch_->bind(7, po.date_);

		if (!po.layout_.empty())
			stmtInsertPatch_->bind(8, po.layout_);
		else
			stmtInsertPatch_->bind(8);

		if (!po.parameters_.empty())
			stmtInsertPatch_->bind(9, po.parameters_);
		else
			stmtInsertPatch_->bind(9);

		if (!po.keyboardBindings_.empty())
			stmtInsertPatch_->bind(10, po.keyboardBindings_);
		else
			stmtInsertPatch_->bind(10);

		if (!po.midiBindings_.empty())
			stmtInsertPatch_->bind(11, po.midiBindings_);
		else
			stmtInsertPatch_->bind(11);
		stmtInsertPatch_->step();
		db_.update("COMMIT;");
	} catch (std::exception& ex) {
		db_.update("ROLLBACK;");
		throw;
	}
}

void SqlStore::selectPatches(const PatchObject& po, std::vector<PatchObject>& result) {
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
		PatchObject po{stmt.column_string(0),
			stmt.column_int64(1),
			stmt.column_string(2),
			stmt.column_string(3),
			stmt.column_string(4),
			stmt.column_string(5),
			stmt.column_int64(6),
			"",
			"",
			"",
			""};

		if(stmt.column_type(7) != sqlite::Statement::NUL)
			po.layout_ = stmt.column_string(7);
		if(stmt.column_type(8) != sqlite::Statement::NUL)
			po.parameters_ = stmt.column_string(8);
		if(stmt.column_type(9) != sqlite::Statement::NUL)
			po.keyboardBindings_ = stmt.column_string(9);
		if(stmt.column_type(10) != sqlite::Statement::NUL)
			po.midiBindings_ = stmt.column_string(10);
		result.push_back(po);
	}
}

void SqlStore::deletePatches(const PatchObject& po) {
	db_.update("BEGIN TRANSACTION;");
	std::ostringstream ssWhere;
	std::ostringstream ssDelete;
	std::ostringstream ssInsert;
	std::vector<string> whereClauses;

	ssDelete << DELETE_PATCHES;
	ssInsert << INSERT_TRASH << SELECT_PATCHES;

	if(!po.isEmpty()) {
		ssWhere << "WHERE ";
		if(!po.name_.empty())
			whereClauses.push_back(string("name == '") + po.name_ + "'");
		if(po.revision_ != -1)
			whereClauses.push_back(string("revision == ") + std::to_string(po.revision_));
		if(!po.runtimeName_.empty())
			whereClauses.push_back(string("runtimeName == '") + po.runtimeName_ + "'");
		if(!po.runtimeVersion_.empty())
			whereClauses.push_back(string("runtimeVersion == '") + po.runtimeVersion_ + "'");
		if(!po.description_.empty())
			whereClauses.push_back(string("description == '") + po.description_ + "'");
		if(!po.code_.empty())
			whereClauses.push_back(string("code == '") + po.code_ + "'");
		if(po.date_ != -1)
			whereClauses.push_back(string("date > ") + std::to_string(po.date_));

		for(size_t i = 0; i < whereClauses.size(); ++i) {
			ssWhere << whereClauses[i];
			if(i < whereClauses.size() - 1)
				ssWhere << " AND ";
		}
	}

	ssWhere << ";";
	ssDelete << ssWhere.str();
	ssInsert << ssWhere.str();

	try {
		db_.update(ssInsert.str());
		db_.update(ssDelete.str());
		db_.update("COMMIT;");
	} catch (std::exception& ex) {
		db_.update("ROLLBACK;");
		throw;
	}
}

void SqlStore::listPatches(std::vector<PatchObject>& result) {
	stmtSelectAllPatches_->reset();

	while (stmtSelectAllPatches_->step() == sqlite::Statement::ROW) {
		PatchObject po{stmtSelectAllPatches_->column_string(0),
			stmtSelectAllPatches_->column_int64(1),
			stmtSelectAllPatches_->column_string(2),
			stmtSelectAllPatches_->column_string(3),
			stmtSelectAllPatches_->column_string(4),
			stmtSelectAllPatches_->column_string(5),
			stmtSelectAllPatches_->column_int64(6),
			"",
			"",
			"",
			""};

		if(stmtSelectAllPatches_->column_type(7) != sqlite::Statement::NUL)
			po.layout_ = stmtSelectAllPatches_->column_string(7);
		if(stmtSelectAllPatches_->column_type(8) != sqlite::Statement::NUL)
			po.parameters_ = stmtSelectAllPatches_->column_string(8);
		if(stmtSelectAllPatches_->column_type(9) != sqlite::Statement::NUL)
			po.keyboardBindings_ = stmtSelectAllPatches_->column_string(9);
		if(stmtSelectAllPatches_->column_type(10) != sqlite::Statement::NUL)
			po.midiBindings_ = stmtSelectAllPatches_->column_string(10);
		result.push_back(po);
	}
}
} /* namespace patchscript */
