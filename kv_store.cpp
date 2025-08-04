#include "kv_store.hpp"

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