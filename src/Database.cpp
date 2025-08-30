#include "Database.hpp"
#include <iostream>

bool Database::createTable(const string& name, const vector<Column>& schema, string& errorMessage)
{
    if (tables.find(name) != tables.end())
    {
        errorMessage = "Table '" + name + "' already exists.";
        return false;
    }
    tables[name] = make_unique<Table>(name, schema);
    return true;
}

bool Database::insertInto(const std::string& tableName, const Row& row, std::string& errorMessage)
{
    auto it = tables.find(tableName);
    if (it == tables.end())
    {
        errorMessage = "Table '" + tableName + "' does not exist.";
        return false;
    }
    return it->second->insert(row, errorMessage);
}

bool Database::selectAll(const std::string& tableName, std::string& errorMessage) const
{
    auto it = tables.find(tableName);
    if (it == tables.end())
    {
        errorMessage = "Table '" + tableName + "' does not exist.";
        return false;
    }
    it->second->display();
    return true;
}

bool Database::tableExists(const std::string& name) const {
    return tables.find(name) != tables.end();
}
