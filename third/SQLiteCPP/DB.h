#ifndef DB_H
#define DB_H

#include "Exception.h"
#include "Statement.h"
#ifdef USE_FTS
#include "Tokenizer.h"
#endif
#include "sqlite3.h"
#include <memory>
#include <string>
#include <unordered_map>

struct sqlite3;
struct sqlite3_context;
struct sqlite3_value;

namespace sqlite {

#ifdef USE_FTS
    extern std::unordered_map<std::string, std::shared_ptr<Tokenizer>> _tokenizers;
    extern sqlite3_tokenizer_module _module;
#endif

    class DB
    {
    public:
        explicit DB(const std::string &filename)
        {
            if (sqlite3_open(filename.c_str(), &_db) != SQLITE_OK)
                throw DBOpenException(_db, "cannot open db");
        }

        ~DB()
        {
            if (_db)
                sqlite3_close(_db);
        }

        Statement prepare(const std::string &sql)
        {
            return Statement(_db, sql);
        }

        template <typename...Args>
        Statement query(const std::string &sql, Args...args)
        {
            auto stmt = prepare(sql);
            if (sizeof...(Args) > 0)
                stmt.bind_all(args...);
            return stmt;
        }

        template <typename...Args>
        bool update(const std::string &sql, Args...args)
        {
            auto stmt = prepare(sql);
            if (sizeof...(Args) > 0)
                stmt.bind_all(args...);
            return stmt.step() == Statement::DONE;
        }

        bool create_function(const std::string &name, int nArg,
                             void(*xFunc)(sqlite3_context*, int, sqlite3_value**))
        {
            sqlite3_create_function(_db, name.c_str(), nArg,
                SQLITE_UTF8|SQLITE_DETERMINISTIC, NULL, xFunc, NULL, NULL);
            return false;
        }

        void exec(const std::string &sql, int(*callback)(void*, int, char**, char**), void *relay)
        {
            auto ret = sqlite3_exec(_db, sql.c_str(), callback, relay, NULL);
            if (ret != SQLITE_OK && ret != SQLITE_ABORT)
                throw DBExecException(_db, "cannot exec " );
        }

        bool close()
        {
            auto ret = sqlite3_close(_db);
            if (ret == SQLITE_OK)
            {
                _db = NULL;
                return true;
            }
            else if (ret == SQLITE_BUSY)
                return false;
            else
                throw DBCloseException(_db, "cannot close db");
        }

#ifdef USE_FTS
        bool add_tokenizer(const std::string &name, std::shared_ptr<Tokenizer> tokenizer)
        {
            if (_tokenizers.find(name) != _tokenizers.end())
                return false;

            const std::string sql = "SELECT fts3_tokenizer(?, ?);";
            auto data = &_module;
            auto stmt = prepare(sql);
            stmt.bind(1, name);
            stmt.bind(2, &data, (int)sizeof(data), Statement::STATIC);

            //Only enable FTS3_TOKENIZER when needed for security purpose
            sqlite3_db_config(_db, SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER, 1, 0);
            stmt.step();
            sqlite3_db_config(_db, SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER, 0, 0);

            _tokenizers[name] = tokenizer;

            return true;
        }
#endif

    private:
        sqlite3 *_db;
    };


}

#endif // DB_H
