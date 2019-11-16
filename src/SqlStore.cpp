#include "SqlStore.hpp"
#include "Exception.h"
#include "fts3_tokenizer.h"
#include "sqlite3.h"
#include "Statement.h"
#include "Tokenizer.h"
#include <sstream>
#include <iostream>

namespace patchscript {

const string CREATE_SESSIONS = R"_STATEMENT_(
		CREATE TABLE IF NOT EXISTS sessions (
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

const string INSERT_SESSION = "INSERT INTO sessions VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
const string INSERT_TRASH = "INSERT INTO trash ";
const string SELECT_ALL_SESSIONS = "SELECT * FROM sessions ORDER BY name, date;";
const string SELECT_SESSIONS = "SELECT * FROM sessions ";
const string SELECT_MAX_REVISION = "SELECT max(revision) FROM sessions WHERE name == ?;";
const string DELETE_SESSIONS = "DELETE FROM sessions ";

SqlStore::SqlStore(const string& dbfile) : db_(dbfile) {
	stmtCreateSessions_ = new sqlite::Statement(db_._db, CREATE_SESSIONS);
	stmtCreateTrash_ = new sqlite::Statement(db_._db, CREATE_TRASH);

	//create session and trash table if the db is empty
	stmtCreateSessions_->step();
	stmtCreateTrash_->step();

	stmtInsertSession_ = new sqlite::Statement(db_._db, INSERT_SESSION);
	stmtSelectAllSessions_ = new sqlite::Statement(db_._db, SELECT_ALL_SESSIONS);
	stmtSelectSessions_ = new sqlite::Statement(db_._db, SELECT_SESSIONS);
	stmtSelectMaxRevision_ = new sqlite::Statement(db_._db, SELECT_MAX_REVISION);
}

SqlStore::~SqlStore() {
	delete(stmtCreateSessions_);
	delete(stmtCreateTrash_);
	delete(stmtInsertSession_);
	delete(stmtSelectAllSessions_);
	delete(stmtSelectSessions_);
	delete(stmtSelectMaxRevision_);
	db_.close();
}

void SqlStore::store(const SessionObject& po) {
	db_.update("BEGIN TRANSACTION;");
	try {
		stmtSelectMaxRevision_->reset();
		stmtSelectMaxRevision_->bind(1, po.name_);

		int64_t revision = 0;
		if (stmtSelectMaxRevision_->step() == sqlite::Statement::ROW) {
			revision = stmtSelectMaxRevision_->column_int64(0) + 1;
		}
		stmtInsertSession_->reset();
		stmtInsertSession_->bind(1, po.name_);
		stmtInsertSession_->bind(2, revision);
		stmtInsertSession_->bind(3, po.runtimeName_);
		stmtInsertSession_->bind(4, po.runtimeVersion_);
		stmtInsertSession_->bind(5, po.description_);
		stmtInsertSession_->bind(6, po.code_);
		stmtInsertSession_->bind(7, po.date_);

		if (!po.layout_.empty())
			stmtInsertSession_->bind(8, po.layout_);
		else
			stmtInsertSession_->bind(8);

		if (!po.parameters_.empty())
			stmtInsertSession_->bind(9, po.parameters_);
		else
			stmtInsertSession_->bind(9);

		if (!po.keyboardBindings_.empty())
			stmtInsertSession_->bind(10, po.keyboardBindings_);
		else
			stmtInsertSession_->bind(10);

		if (!po.midiBindings_.empty())
			stmtInsertSession_->bind(11, po.midiBindings_);
		else
			stmtInsertSession_->bind(11);
		stmtInsertSession_->step();
		db_.update("COMMIT;");
	} catch (std::exception& ex) {
		db_.update("ROLLBACK;");
		throw;
	}
}

void SqlStore::select(const SessionObject& po, std::vector<SessionObject>& result) {
	std::ostringstream ss;
	ss << SELECT_SESSIONS;
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
		SessionObject po{stmt.column_string(0),
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

void SqlStore::remove(const SessionObject& po) {
	db_.update("BEGIN TRANSACTION;");
	std::ostringstream ssWhere;
	std::ostringstream ssDelete;
	std::ostringstream ssInsert;
	std::vector<string> whereClauses;

	ssDelete << DELETE_SESSIONS;
	ssInsert << INSERT_TRASH << SELECT_SESSIONS;

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

void SqlStore::list(std::vector<SessionObject>& result) {
	stmtSelectAllSessions_->reset();

	while (stmtSelectAllSessions_->step() == sqlite::Statement::ROW) {
		SessionObject po{stmtSelectAllSessions_->column_string(0),
			stmtSelectAllSessions_->column_int64(1),
			stmtSelectAllSessions_->column_string(2),
			stmtSelectAllSessions_->column_string(3),
			stmtSelectAllSessions_->column_string(4),
			stmtSelectAllSessions_->column_string(5),
			stmtSelectAllSessions_->column_int64(6),
			"",
			"",
			"",
			""};

		if(stmtSelectAllSessions_->column_type(7) != sqlite::Statement::NUL)
			po.layout_ = stmtSelectAllSessions_->column_string(7);
		if(stmtSelectAllSessions_->column_type(8) != sqlite::Statement::NUL)
			po.parameters_ = stmtSelectAllSessions_->column_string(8);
		if(stmtSelectAllSessions_->column_type(9) != sqlite::Statement::NUL)
			po.keyboardBindings_ = stmtSelectAllSessions_->column_string(9);
		if(stmtSelectAllSessions_->column_type(10) != sqlite::Statement::NUL)
			po.midiBindings_ = stmtSelectAllSessions_->column_string(10);
		result.push_back(po);
	}
}
} /* namespace patchscript */
