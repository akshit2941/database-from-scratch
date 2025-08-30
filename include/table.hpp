#pragma once
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

enum class DataTypeTable{
    INT,
    STRING
};

enum class Constraint{
    NONE,
    NOT_NULL
};

struct Column{
    string name;
    DataTypeTable type;
    Constraint constraint;

    Column(string name, DataTypeTable type, Constraint constraint = Constraint::NONE);
};

using Row = unordered_map<string, string>;

class Table{
private:
    string name;
    vector<Column> schema;
    vector<Row> rows;

    const Column* getColumn(const string& colName) const;
    bool validateRow(const Row& row, string& errorMessage) const;

public:
    Table(string name, vector<Column> schema);

    bool insert(const Row& row, string& errorMessage);
    void display() const;
};