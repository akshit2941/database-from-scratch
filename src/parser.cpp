#include "parser.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

CommandType Parser::identifyCommand(const string& word){
    if(word=="CREATE") return CommandType::CREATE;
    if(word=="INSERT") return CommandType::INSERT;
    if(word=="SELECT") return CommandType::SELECT;
    return CommandType::UNKNOWN;
}

DataType Parser::identifyDataType(const string& word){
    if(word=="INT") return DataType::INT;
    if(word=="STRING") return DataType::STRING;
    return DataType::INVALID;
}

string Parser::removeSemicolon(const string& word){
    if(!word.empty() && word.back() == ';'){
        return word.substr(0,word.size()-1);
    }
    return word;
}

vector<string> Parser::tokenize(const string& input){
    vector<string> tokens;
    string token;
    stringstream ss(input);

    while(ss>>token){
        tokens.push_back(removeSemicolon(token));
    }

    return tokens;
}

ParsedCommand Parser::parse(const string& input){
    ParsedCommand result;
    auto tokens = tokenize(input);

    if(tokens.empty()){
        result.commandType = CommandType::UNKNOWN;
        return result;
    }

    result.commandType = identifyCommand(tokens[0]);

    if(result.commandType == CommandType::CREATE){
        if(tokens.size() < 4 || tokens[1]!="TABLE") return result;

        result.tableName = tokens[2];

        string rawCols;
        size_t open = input.find('(');
        size_t closed = input.find(')');

        if(open == string::npos || closed == string::npos) return result;

        rawCols = input.substr(open+1, closed-open+1);
        stringstream ss(rawCols);
        string segment;

        while(getline(ss, segment, ',')){
            stringstream pair(segment);
            string name, dtype;
            pair >> name >> dtype;

            ColumnDef col;
            col.name = name;
            col.type = identifyDataType(dtype);
            result.columns.push_back(col);
        }
    }else if(result.commandType == CommandType::INSERT){
        if(tokens.size() < 5 || tokens[1]!= "INTO" || tokens[3]!= "VALUES") return result;

        result.tableName = tokens[2];
        
        size_t open = input.find('(');
        size_t close = input.find(')');

        if(open == string::npos || close == string::npos) return result;

        string rawValues = input.substr(open+1, close-open-1);
        stringstream ss(rawValues);
        string value;

        while(getline(ss,value,',')){
            value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());
            result.values.push_back(value);
        }
    }else if(result.commandType == CommandType::SELECT){
        if(tokens.size()>=4 && tokens[1]=="*" && tokens[2]=="FROM"){
            result.tableName = tokens[3];
        }
    }

    return result;
}