#include <iostream>
#include "HashTable.hpp"

std::vector<int> generate_keys(size_t count) {
    std::vector<int> keys;
    keys.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        // диапазон ключей: [1, count*10]
        int key = 1 + (std::rand() % (count * 10));
        keys.push_back(key);
    }
    return keys;
}


int main() {
    HashTable ht(20);
    auto keys = generate_keys(10);

    for (auto key : keys) {
        ht.insert(key, std::to_string(key));
    }

    ht.print();
}