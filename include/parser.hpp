#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

enum class CommandType{
    CREATE,
    INSERT,
    SELECT,
    UNKNOWN
};

enum class DataType{
    INT,
    STRING,
    INVALID
};

struct ColumnDef{
    string name;
    DataType type;
};

struct ParsedCommand{
    CommandType commandType;
    string tableName;
    vector<ColumnDef> columns;
    vector<string> values;
};

class Parser{
public:
    ParsedCommand parse(const string& word);
private:
    CommandType identifyCommand(const string& word);
    DataType identifyDataType(const string& word);
    vector<string>tokenize(const string& word);
    string removeSemicolon(const string& word);
};

#endif