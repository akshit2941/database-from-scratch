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
    if(!word.empty() && word.back() == ";"){
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