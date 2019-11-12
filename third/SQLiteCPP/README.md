#  SQLiteCPP
A SQLite C++ Wrapper. 



## Basic Usage

Clone this repo, add all header files into your project is enough.


1. Query

```c++
//Suppose database name is "my.db". 
//This db has a table "mytable". 
//"mytable" has 3 columns:id int, name varchar(20), grade int
sqlite::DB db("./my.db");
std::string sql = "SELECT * FROM mytable WHERE grade > ? AND name LIKE ?";
auto stmt = db.query(sql, 88, "Jord%");
while (stmt.step() == sqlite::Statement::ROW)
{
    std::cout << stmt.column_int(0)    << " "
              << stmt.column_string(1) << " "
              << stmt.column_int(2)    << std::endl;
}
```



2. Update

```c++
sqlite::DB db("./my.db");
std::string sql = "INSERT INTO mytable VALUES(?, ?, ?)";
db.update(sql, 3, "Jordan", 88);
```



3. Bulk update using transaction

```c++
sqlite::DB db("./my.db");
std::string sql = "INSERT INTO mytable VALUES(?, ?, ?)";
db.update("BEGIN TRANSACTION;");
auto stmt = db.prepare(sql);
for (auto &i : students)
{
    stmt.bind_all(i.getId(), i.getName(), i.getGrade());
    stmt.step();
    stmt.reset();
}
db.update("COMMIT;");
```





## Advanced Usage: Full text search

To use full text search:

1. Add both `Tokenizer.h` and `Tokenizer.cpp` to your project.
2. Define `USE_FTS` macro.

(Optinal) To use cppjieba tokenizer, you should clone cppJieba tokenizer repo and add following path to your include path:

```
PATH_TO_JIEBA_REPO/cppjieba/include
PATH_TO_JIEBA_REPO/cppjieba/deps
```

This is an example:

```c++
sqlite::DB db("./my.db");

//Initialize custom tokenizer
auto tknz = make_shared<MyTokenizer>("PATH_TO_JIEBA_REPO");

//Use external Jieba tokenizer, if you use integrated tokenizers like `porter`, this line is not needed
db.add_tokenizer("jieba", tknz); 

//Create rank function, use TF-IDF based ranking
db.create_function("rank", -1, rankfunc);

//SQL string used to create virtual table using Jieba tokenizer. Note: tokenizer need to repeat twice
std::string sql_create = "CREATE VIRTUAL TABLE IF NOT EXISTS my_index USING fts4(id, name, grade, tokenize=jieba jieba)";
std::string sql_query = "SELECT id,name,grade,rank,snippet(my_index, \"<font color=red>\", \"</font>\", \"...\", -1, 11) AS snippet FROM my_index JOIN (SELECT docid, rank(matchinfo(my_index,'pcxnl'),1,1,1) AS rank FROM my_index WHERE my_index MATCH ? ORDER BY rank DESC ) AS ranktable USING(docid) WHERE my_index MATCH ? ORDER BY ranktable.rank DESC";

auto stmt = db.query(sql_query, "我", "我"); //Keyword needs to be repeat twice due to the form of query sql
while (stmt.step() == sqlite::Statement::ROW)
{
    std::cout << stmt.column_int(0)    << " " //id
              << stmt.column_string(1) << " " //name
              << stmt.column_int(2)    << " " //grade
              << stmt.column_double(3) << " " //rank, rank score of this row, higher is better
              << stmt.column_string(4) << " " //snippet, short html string that contains keyword
              << std::endl;
}
```

The SQL string is quite complex, for more details please refer to SQLite document [Appendix A: Search Application Tips](https://www.sqlite.org/fts3.html#appendix_a)

