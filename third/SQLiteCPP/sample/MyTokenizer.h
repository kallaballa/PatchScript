#pragma once

#include "../Tokenizer.h"
#include <memory>
#include <vector>
#include <string>


namespace cppjieba {
    class QuerySegment;
}

// Custom tokenizer using Jieba Tokenizer
class MyTokenizer :
    public sqlite::Tokenizer
{
public:
    MyTokenizer(const std::string &dict_path);
    ~MyTokenizer() override;

    int open(sqlite::Cursor &cursor) override;
    int close(sqlite::Cursor &cursor) override;
    bool next(sqlite::Cursor &cursor) override;

private:
    std::unique_ptr<cppjieba::QuerySegment> querySegment;
};

