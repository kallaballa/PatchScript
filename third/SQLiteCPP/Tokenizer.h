#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>

struct sqlite3_tokenizer_module;
struct sqlite3_tokenizer;
struct sqlite3_tokenizer_cursor;

namespace sqlite {

    struct Cursor
    {
        void *context;
        std::string input;
        char token_str[128];
        int bytes;
        int start;
        int end;
        int position;
    };

    class Tokenizer
    {
    public:
        Tokenizer();
        virtual ~Tokenizer();
        virtual int open(Cursor &cursor) = 0;
        virtual int close(Cursor &cursor) = 0;
        virtual bool next(Cursor &cursor) = 0;
    };
}

#endif // TOKENIZER_H
