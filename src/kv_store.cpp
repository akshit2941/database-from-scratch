#include "kv_store.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

void KeyValueStore::put(const string &key, const string &value){
    store[key] = value;
}

string KeyValueStore::get(const string &key){
    if (store.find(key) != store.end())
    {
        return store[key];
    }
    else
    {
        return "";
    }
}

void KeyValueStore::remove(const string &key){
    store.erase(key);
}

void KeyValueStore::loadFromFile(const string& filePath){
    ifstream file(filePath);
    if(!file.is_open()){
        cerr << "No existing database found!\n";
        cerr << "Creating fresh\n";
        return;
    }

    string line;
    while(getline(file,line)){
        istringstream iss(line);
        string op, key, value;

        iss>> op >> key;
        getline(iss, value);

        if(!value.empty() && value[0] == ' ') value.erase(0,1);

        if(op == "PUT"){
            store[key] = value;
        }else if(op=="REMOVE"){
            store.erase(key);
        }
    }
    file.close();
}

void KeyValueStore::appendToFile(const string& filePath, const string& op, const string& key, const string& value){
    ofstream file(filePath,ios::app);
    if(!file.is_open()){
        cerr<<"Failed to open database.\n";
        return;
    }

    if(op=="PUT"){
        file<<"PUT "<<key <<" "<< value << "\n";
    }else if(op=="REMOVE"){
        file<<"REMOVE "<<key<<"\n";
    }

    file.close();
}