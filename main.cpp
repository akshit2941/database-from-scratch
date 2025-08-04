#include <iostream>
#include <string>
#include <sstream>
#include "kv_store.hpp"

using namespace std;

int main()
{
    KeyValueStore kv;

    string input;
    cout << "Welcome to the Db\n";
    cout << "Available commands: PUT, GET, REMOVE, EXIT\n";

    while (true)
    {
        cout << ">> ";
        getline(cin, input);

        istringstream iss(input);
        string command;
        iss >> command;

        if (command == "put" || "add")
        {
            string key, value;
            iss >> key >> ws;

            getline(iss, value);

            string keyValue = kv.get(key);
            if (!keyValue.empty())
            {
                cout << "Value already exist wiht " << key << "!\n";
                cout << "Try changing the key!\n";
            }
            else
            {
                kv.put(key, value);
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
                cout << "REMOVED : " << value << "\n";
            }
            else
            {
                cout << "No KEY matching with : " << key << "\n";
            }
        }
        else if (command == "exit")
        {
            cout << "ABORTING!";
            break;
        }
        else
        {
            cout << "INVALID COMMAND!, Please use put, get, remove, exit\n";
        }
    }
}
