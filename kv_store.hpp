#ifndef KV_STORE_HPP
#define KV_STORE_HPP

#include <string>
#include <unordered_map>

using namespace std;

class KeyValueStore{
private:
    unordered_map<string,string> store;

public:
    void put(const string& key, const string& value);

    string get(const string& key);

    void remove(const string& key);

    void loadFromFile(const string& filePath);

    void appendToFile(const string& filePath, const string& op, const string& key, const string& value="");
};

#endif