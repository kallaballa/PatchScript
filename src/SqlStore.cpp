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
	    author TEXT NOT NULL,
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
			PRIMARY KEY(name,author,revision)
		);
		)_STATEMENT_";

const string CREATE_TRASH = R"_STATEMENT_(
		CREATE TABLE IF NOT EXISTS trash (
	    name TEXT NOT NULL,
	    author TEXT NOT NULL,
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

const string INSERT_SESSION = "INSERT INTO sessions VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
const string INSERT_TRASH = "INSERT INTO trash ";
const string SELECT_ALL_SESSIONS = "SELECT * FROM sessions ORDER BY name, author, date;";
const string SELECT_SESSIONS = "SELECT * FROM sessions ";
const string SELECT_MAX_REVISION = "SELECT max(revision) FROM sessions WHERE name == ? and author == ?;";
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

void SqlStore::store(const SessionObject& so) {
	db_.update("BEGIN TRANSACTION;");
	try {
		stmtSelectMaxRevision_->reset();
		stmtSelectMaxRevision_->bind(1, so.name_);

		int64_t revision = 0;
		if (stmtSelectMaxRevision_->step() == sqlite::Statement::ROW) {
			revision = stmtSelectMaxRevision_->column_int64(0) + 1;
		}
		stmtInsertSession_->reset();
		stmtInsertSession_->bind(1, so.name_);
		stmtInsertSession_->bind(2, so.author_);
		stmtInsertSession_->bind(3, revision);
		stmtInsertSession_->bind(4, so.runtimeName_);
		stmtInsertSession_->bind(5, so.runtimeVersion_);
		stmtInsertSession_->bind(6, so.description_);
		stmtInsertSession_->bind(7, so.code_);
		stmtInsertSession_->bind(8, so.date_);

		if (!so.layout_.empty())
			stmtInsertSession_->bind(9, so.layout_);
		else
			stmtInsertSession_->bind(9);

		if (!so.parameters_.empty())
			stmtInsertSession_->bind(10, so.parameters_);
		else
			stmtInsertSession_->bind(10);

		if (!so.keyboardBindings_.empty())
			stmtInsertSession_->bind(11, so.keyboardBindings_);
		else
			stmtInsertSession_->bind(11);

		if (!so.midiBindings_.empty())
			stmtInsertSession_->bind(12, so.midiBindings_);
		else
			stmtInsertSession_->bind(12);
		stmtInsertSession_->step();
		db_.update("COMMIT;");
	} catch (std::exception& ex) {
		db_.update("ROLLBACK;");
		throw;
	}
}
void SqlStore::remove(const SessionObject& so) {
	db_.update("BEGIN TRANSACTION;");
	std::ostringstream ssWhere;
	std::ostringstream ssDelete;
	std::ostringstream ssInsert;
	std::vector<string> whereClauses;

	ssDelete << DELETE_SESSIONS;
	ssInsert << INSERT_TRASH << SELECT_SESSIONS;

	if(!so.isEmpty()) {
		ssWhere << "WHERE ";
		if(!so.name_.empty())
			whereClauses.push_back(string("name == '") + so.name_ + "'");
		if(!so.author_.empty())
			whereClauses.push_back(string("author == '") + so.author_ + "'");
		if(so.revision_ != -1)
			whereClauses.push_back(string("revision == ") + std::to_string(so.revision_));
		if(!so.runtimeName_.empty())
			whereClauses.push_back(string("runtimeName == '") + so.runtimeName_ + "'");
		if(!so.runtimeVersion_.empty())
			whereClauses.push_back(string("runtimeVersion == '") + so.runtimeVersion_ + "'");
		if(!so.description_.empty())
			whereClauses.push_back(string("description == '") + so.description_ + "'");
		if(!so.code_.empty())
			whereClauses.push_back(string("code == '") + so.code_ + "'");
		if(so.date_ != -1)
			whereClauses.push_back(string("date > ") + std::to_string(so.date_));

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
			stmtSelectAllSessions_->column_string(1),
			stmtSelectAllSessions_->column_int64(2),
			stmtSelectAllSessions_->column_string(3),
			stmtSelectAllSessions_->column_string(4),
			stmtSelectAllSessions_->column_string(5),
			stmtSelectAllSessions_->column_string(6),
			stmtSelectAllSessions_->column_int64(7),
			"",
			"",
			"",
			""};

		if(stmtSelectAllSessions_->column_type(8) != sqlite::Statement::NUL)
			po.layout_ = stmtSelectAllSessions_->column_string(8);
		if(stmtSelectAllSessions_->column_type(9) != sqlite::Statement::NUL)
			po.parameters_ = stmtSelectAllSessions_->column_string(9);
		if(stmtSelectAllSessions_->column_type(10) != sqlite::Statement::NUL)
			po.keyboardBindings_ = stmtSelectAllSessions_->column_string(10);
		if(stmtSelectAllSessions_->column_type(11) != sqlite::Statement::NUL)
			po.midiBindings_ = stmtSelectAllSessions_->column_string(11);
		result.push_back(po);
	}
}
} /* namespace patchscript */
