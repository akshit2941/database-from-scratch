#include <iostream>
#include <string>
#include <sstream>
#include "kv_store.hpp"
#include "parser.hpp"
#include "Database.hpp"

using namespace std;

vector<Column> convertColumns(const vector<ColumnDef>& colDefs) {
    vector<Column> columns;
    for (const auto& def : colDefs) {
        DataTypeTable type = (def.type == DataType::INT) ? DataTypeTable::INT : DataTypeTable::STRING;
        columns.emplace_back(def.name, type);
    }
    return columns;
}

Row convertToRow(const vector<string>& values, const vector<ColumnDef>& colDefs) {
    Row row;
    for (size_t i = 0; i < values.size() && i < colDefs.size(); i++) {
        row[colDefs[i].name] = values[i];
    }
    return row;
}

int main()
{
    KeyValueStore kv;
    string dbFile = "data.db";

    kv.loadFromFile(dbFile);

    Database db;
    Parser parser;

    string input;
    cout << "Available commands:\n";
    cout << "   KV: PUT, GET, REMOVE, EXIT\n";
    cout << "   SQL: CREATE TABLE, INSERT INTO, SELECT, DELETE\n\n";

    while (true)
    {
        cout << ">> ";
        getline(cin, input);

        // check if it's a SQL-like command
        ParsedCommand cmd = parser.parse(input);

        if (cmd.commandType != CommandType::UNKNOWN)
        {
            if (cmd.commandType == CommandType::CREATE)
            {
                if (db.tableExists(cmd.tableName))
                {
                    cout << "Table already exists: " << cmd.tableName << "\n";
                }
                else
                {
                    string errorMsg;
                    vector<Column> columns = convertColumns(cmd.columns);
                    if (db.createTable(cmd.tableName, columns, errorMsg)) {
                        cout << "Table created: " << cmd.tableName << "\n";
                    } else {
                        cout << "Failed to create table: " << errorMsg << "\n";
                    }
                }
            }
            else if (cmd.commandType == CommandType::INSERT)
            {
                if (!db.tableExists(cmd.tableName))
                {
                    cout << "Table not found: " << cmd.tableName << "\n";
                }
                else
                {
                    try
                    {
                        string errorMsg;
                        Row row = convertToRow(cmd.values, cmd.columns);
                        if (db.insertInto(cmd.tableName, row, errorMsg)) {
                            cout << "Row inserted into " << cmd.tableName << "\n";
                        } else {
                            cout << "Insert failed: " << errorMsg << "\n";
                        }
                    }
                    catch (const exception &e)
                    {
                        cout << "Insert failed: " << e.what() << "\n";
                    }
                }
            }
            else if (cmd.commandType == CommandType::SELECT)
            {
                if (!db.tableExists(cmd.tableName))
                {
                    cout << "Table not found: " << cmd.tableName << "\n";
                }
                else
                {
                    string errorMsg;
                    db.selectAll(cmd.tableName, errorMsg);
                }
            }
        }
        else
        {
            // fallback to old key-value commands
            istringstream iss(input);
            string command;
            iss >> command;

            for (auto &ch : command)
                ch = tolower(ch);

            if (command == "put" || command == "add")
            {
                string key, value;
                iss >> key >> ws;
                getline(iss, value);

                string keyValue = kv.get(key);
                if (!keyValue.empty())
                {
                    cout << "Value already exists with key " << key << "!\n";
                }
                else
                {
                    kv.put(key, value);
                    kv.appendToFile(dbFile, "PUT", key, value);
                    cout << "Added " << key << " : " << value << "\n";
                }
            }
            else if (command == "get")
            {
                string key;
                iss >> key;
                string value = kv.get(key);
                if (value.empty())
                {
                    cout << "NOT_FOUND\n";
                }
                else
                {
                    cout << value << "\n";
                }
            }
            else if (command == "remove")
            {
                string key;
                iss >> key;
                string value = kv.get(key);
                if (!value.empty())
                {
                    kv.remove(key);
                    kv.appendToFile(dbFile, "REMOVE", key);
                    cout << "REMOVED : " << value << "\n";
                }
                else
                {
                    cout << "No KEY matching with : " << key << "\n";
                }
            }
            else if (command == "exit")
            {
                cout << "ABORTING!\n";
                break;
            }
            else
            {
                cout << "INVALID COMMAND! Use put, get, remove, exit or SQL-like commands (CREATE, INSERT, SELECT).\n";
            }
        }
    }
    return 0;
}