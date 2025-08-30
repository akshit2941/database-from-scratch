#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "table.hpp"

class Database
{
    unordered_map<string,unique_ptr<Table>> tables;

public:
    bool createTable(const string& name, const vector<Column>& schema, string& errorMessage);
    bool insertInto(const string& tableName, const Row& row, string& errorMessage);
    bool selectAll(const string& tableName, string& errorMessage) const;
    bool tableExists(const std::string& name) const;
};