#ifndef STATEMENT_H
#define STATEMENT_H

#include "Exception.h"
#include "sqlite3.h"

#include <cstdint>
#include <string>
#include <tuple>

struct sqlite3_value;
struct sqlite3_stmt;
struct sqlite3;

namespace sqlite {
    class Statement
    {
    public:
        enum State { BUSY, ROW, DONE, MISUSE };
        enum DataType { INTEGER = 1, FLOAT, TEXT, BLOB, NUL };
        enum DestructorType { STATIC, TRANSIENT };

        Statement(sqlite3 *db, const std::string &sql)
            : _db(db), _sql(sql)
        {
            if (sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &_stmt, NULL) != SQLITE_OK)
                throw PrepareException(db, std::string("cannot prepare ") + sql);
        }

        Statement(Statement &&other)
            :_db(other._db), _stmt(other._stmt), _sql(std::move(other._sql))
        {
            other._db = NULL;
            other._stmt = NULL;
        }

        ~Statement()
        {
            finalize();
        }

        void finalize()
        {
            sqlite3_finalize(_stmt);
            _stmt = NULL;
        }

        void bind(int idx, const void *blob, int n, DestructorType dtor)
        {
            auto d = dtor == STATIC ? SQLITE_STATIC : SQLITE_TRANSIENT;
            if (sqlite3_bind_blob(_stmt, idx, blob, n, d) != SQLITE_OK)
                throw BindException(_db, "cannot bind");
        }

        void bind(int idx, const void *blob, uint64_t n, DestructorType dtor)
        {
            auto d = dtor == STATIC ? SQLITE_STATIC : SQLITE_TRANSIENT;
            if (sqlite3_bind_blob64(_stmt, idx, blob, n, d) != SQLITE_OK)
                throw BindException(_db, "cannot bind");
        }

        void bind(int idx, double value)
        {
            if (sqlite3_bind_double(_stmt, idx, value) != SQLITE_OK)
                throw BindException(_db, "cannot bind");
        }

        void bind(int idx, int value)
        {
            if (sqlite3_bind_int(_stmt, idx, value) != SQLITE_OK)
                throw BindException(_db, "cannot bind");
        }

        void bind(int idx, int64_t value)
        {
            if (sqlite3_bind_int64(_stmt, idx, value) != SQLITE_OK)
                throw BindException(_db, "cannot bind");
        }

        void bind(int idx)
        {
            if (sqlite3_bind_null(_stmt, idx) != SQLITE_OK)
                throw BindException(_db, "cannot bind");
        }

        void bind(int idx, const std::string &text)
        {
            if (sqlite3_bind_text(_stmt, idx, text.c_str(), text.size(), SQLITE_TRANSIENT) != SQLITE_OK)
                throw BindException(_db, "cannot bind");
        }

        void bind(int idx, const char *text, int n, DestructorType dtor)
        {
            auto d = dtor == STATIC ? SQLITE_STATIC : SQLITE_TRANSIENT;
            if (sqlite3_bind_text(_stmt, idx, text, n, d) != SQLITE_OK)
                throw BindException(_db, "cannot bind");
        }

        void bind(int idx, const char *text, uint64_t n, DestructorType dtor, unsigned char encoding)
        {
            auto d = dtor == STATIC ? SQLITE_STATIC : SQLITE_TRANSIENT;
            if (sqlite3_bind_text64(_stmt, idx, text, n, d, encoding) != SQLITE_OK)
                throw BindException(_db, "cannot bind");
        }

        void bind(int idx, const sqlite3_value *value)
        {
            if (sqlite3_bind_value(_stmt, idx, value) != SQLITE_OK)
                throw BindException(_db, "cannot bind");
        }

        void bind(int idx, void *ptr, const char *type, DestructorType dtor)
        {
            auto d = dtor == STATIC ? SQLITE_STATIC : SQLITE_TRANSIENT;
            if (sqlite3_bind_pointer(_stmt, idx, ptr, type, d) != SQLITE_OK)
                throw BindException(_db, "cannot bind");
        }

        void bind_zeroblob(int idx, int n)
        {
            if (sqlite3_bind_zeroblob(_stmt, idx, n) != SQLITE_OK)
                throw BindException(_db, "cannot bind");
        }

        void bind_zeroblob64(int idx, uint64_t n)
        {
            if (sqlite3_bind_zeroblob64(_stmt, idx, n) != SQLITE_OK)
                throw BindException(_db, "cannot bind");
        }

        template <typename ...Args>
        void bind_all(Args...args)
        {
            _bind(1, args...);
        }

        State step()
        {
            auto ret = sqlite3_step(_stmt);
            switch (ret)
            {
            case SQLITE_ROW:
                return ROW;
            case SQLITE_DONE:
                return DONE;
            case SQLITE_BUSY:
                return BUSY;
            case SQLITE_MISUSE:
                return MISUSE;
            default:
                throw StepException(_db, "cannot step");
            }
        }

        void reset()
        {
            if (sqlite3_reset(_stmt) != SQLITE_OK)
                throw ResetException(_db, "cannot reset");
        }

        const void* column_blob(int iCol)
        {
            return sqlite3_column_blob(_stmt, iCol);
        }

        double column_double(int iCol)
        {
            return sqlite3_column_double(_stmt, iCol);
        }

        int column_int(int iCol)
        {
            return sqlite3_column_int(_stmt, iCol);
        }

        int64_t column_int64(int iCol)
        {
            return sqlite3_column_int64(_stmt, iCol);
        }

        const unsigned char* column_text(int iCol)
        {
            return sqlite3_column_text(_stmt, iCol);
        }

        std::string column_string(int iCol)
        {
            return std::string(reinterpret_cast<const char*>(sqlite3_column_text(_stmt, iCol)));
        }

        const void *column_text16(int iCol)
        {
            return sqlite3_column_text16(_stmt, iCol);
        }

        sqlite3_value* column_value(int iCol)
        {
            return sqlite3_column_value(_stmt, iCol);
        }

//        template <typename ...Args>
//        std::tuple<Args...> column_all()
//        {
//            std::tuple<Args...> values;
//            _column<0>(values);
//            return values;
//        }

        const char* column_name(int N)
        {
            return sqlite3_column_name(_stmt, N);
        }

        DataType column_type(int iCol)
        {
            return (DataType)sqlite3_column_type(_stmt, iCol);
        }

        int column_bytes(int iCol)
        {
            return sqlite3_column_bytes(_stmt, iCol);
        }

        int column_bytes16(int iCol)
        {
            return sqlite3_column_bytes16(_stmt, iCol);
        }

        int column_count()
        {
            return  sqlite3_column_count(_stmt);
        }

    private:
        sqlite3 *_db;
        sqlite3_stmt *_stmt;
        const std::string _sql;

        void _bind(int) {}

        template <typename T, typename ...Args>
        void _bind(int idx, T value, Args...args)
        {
            bind(idx, value);
            _bind(idx + 1, args...);
        }

//        template <size_t index, typename ...Args>
//        typename std::enable_if<index == sizeof...(Args)>::type
//        _column(std::tuple<Args...> &) {}

//        template <size_t index, typename ...Args>
//        typename std::enable_if<(index < sizeof...(Args))>::type
//        _column(std::tuple<Args...> &values)
//        {
//            _column_value(index, std::get<index>(values));
//            _column<index + 1>(values);
//        }

//        template <typename T>
//        void _column_value(int iCol, T &value)
//        {
//            value = std::move(column<T>(iCol));
//        }
    };
}

#endif // STATEMENT_H
