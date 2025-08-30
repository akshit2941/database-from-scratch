#include "table.hpp"
#include <iostream>
#include <sstream>
#include <cctype>


Column::Column(string name, DataTypeTable type, Constraint constraint)
    : name(name), type(type), constraint(constraint) {}

const Column* Table::getColumn(const string& colName) const {
    for (const auto& col : schema) {
        if (col.name == colName)
            return &col;
    }
    return nullptr;
}

bool isInteger(const string& value) {
    if (value.empty()) return false;
    for (char c : value) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool Table::validateRow(const Row& row, string& errorMessage) const {
    for (const auto& col : schema) {
        auto it = row.find(col.name);
        if (it == row.end()) {
            if (col.constraint == Constraint::NOT_NULL) {
                errorMessage = "Column '" + col.name + "' is NOT NULL but missing.";
                return false;
            }
            continue;
        }

        const string& value = it->second;

        if (col.constraint == Constraint::NOT_NULL && value.empty()) {
            errorMessage = "Column '" + col.name + "' cannot be NULL.";
            return false;
        }

        if (col.type == DataTypeTable::INT && !isInteger(value)) {
            errorMessage = "Column '" + col.name + "' expects an INT, but got '" + value + "'";
            return false;
        }

    }

    return true;
}

Table::Table(string name, vector<Column> schema)
    : name(name), schema(schema) {}

bool Table::insert(const Row& row, string& errorMessage) {
    if (!validateRow(row, errorMessage)) {
        return false;
    }
    rows.push_back(row);
    return true;
}

void Table::display() const {
    cout << "Table: " << name << "\n";

    for (const auto& col : schema) {
        cout << col.name << "\t";
    }
    cout << "\n";

    for (const auto& row : rows) {
        for (const auto& col : schema) {
            auto it = row.find(col.name);
            if (it != row.end()) {
                cout << it->second << "\t";
            } else {
                cout << "NULL\t";
            }
        }
        cout << "\n";
    }
}
