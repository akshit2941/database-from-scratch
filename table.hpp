#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>

using namespace std;

enum class DataType{
    INT,
    STRING
};

enum class Constraint{
    NONE,
    NOT_NULL
};

struct Column{
    string name;
    DataType type;
    Constraint constraint;

    Column(string name, DataType type, Constraint constraint = Constraint::NONE);
};

using Row = unordered_map<string, string>;

class Table{
private:
    string name;
    vector<Column> schema;
    vector<Row> row;

    const Column* getColumn(const string& colName) const;
    bool validateRow(const Row& row, string& errorMessage) const;

public:
    Table(string name, vector<Column> schema);

    bool insert(const Row& row, string& errorMessage);
    void display() const;
}