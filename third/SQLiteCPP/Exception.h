#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "sqlite3.h"
#include <stdexcept>

struct sqlite3;

namespace sqlite {

    class DBOpenException : public virtual std::runtime_error
    {
    public:
        DBOpenException(sqlite3 *db, const std::string &message)
            : std::runtime_error(message + sqlite3_errmsg(db)) {}
    };

    class DBCloseException : public virtual std::runtime_error
    {
    public:
        DBCloseException(sqlite3 *db, const std::string &message)
            : std::runtime_error(message + sqlite3_errmsg(db)) {}
    };

    class DBExecException : public virtual std::runtime_error
    {
    public:
        DBExecException(sqlite3 *db, const std::string &message)
            : std::runtime_error(message + sqlite3_errmsg(db)) {}
    };

    class PrepareException : public virtual std::runtime_error
    {
    public:
        PrepareException(sqlite3 *db, const std::string &message)
            : std::runtime_error(message + sqlite3_errmsg(db)) {}
    };

    class BindException : public virtual std::runtime_error
    {
    public:
        BindException(sqlite3 *db, const std::string &message)
            : std::runtime_error(message + sqlite3_errmsg(db)) {}
    };

    class StepException : public virtual std::runtime_error
    {
    public:
        StepException(sqlite3 *db, const std::string &message)
            : std::runtime_error(message + sqlite3_errmsg(db)) {}
    };

    class ResetException : public virtual std::runtime_error
    {
    public:
        ResetException(sqlite3 *db, const std::string &message)
            : std::runtime_error(message + sqlite3_errmsg(db)) {}
    };
}

#endif // EXCEPTION_H
