#include "HashTable.hpp"
#include <cmath>
#include <iostream>

HashTable::HashTable(size_t size, TypeHashTable typeHT) noexcept
                    : _capacity(size), _filled(0), _typeHT(typeHT) {
    table.resize(_capacity);
}

HashTable::~HashTable() = default;


void HashTable::insert(const int key, const std::string& value)
{
    std::pair<int, std::string> kVPair = {key, value};
    size_t id = hash_function(key);

    switch (_typeHT) {
        case TypeHashTable::Sorted: {
            auto& chain = table[id];
            // Ищем позицию для вставки или замены
            auto it = chain.begin();
            while (it != chain.end() && it->first < key) {
                ++it;
            }
            if (it != chain.end() && it->first == key) {
                it->second = value;  // замена
                return;
            }
            chain.insert(it, kVPair);  // вставка перед it (сохраняя порядок)
            ++_filled;
            break;
        }
        default: {//unsorted
            auto& chain = table[id];
            for (auto& p : chain) {
                if (p.first == key) {
                    p.second = value;
                    return;
                }
            }
            chain.push_front({key, value});
            ++_filled; 
            break;
        }
    }
    
    if (getLoadFactor() > 3.0/4.0) {
        resize();
    }
}


bool HashTable::find(const int key, std::string &value) const {
    size_t id = hash_function(key);

    for (auto it = table[id].begin(); it != table[id].end(); ++it) {
        if (it->first == key) {
            value = it->second;
            return true;
        }
    }

    return false;
}


void HashTable::remove(const int key) {
    size_t id = hash_function(key);
    size_t erased = 0;
    for (auto it = table[id].begin(); it != table[id].end(); ) {
        if (it->first == key) {
            it = table[id].erase(it);
            ++erased;
        } else {
            ++it;
        }
    }
    _filled -= erased;
}
 
std::string& HashTable::operator[](const int key) {
    if (getLoadFactor() > 0.75) {
        resize();  
    }

    size_t id = hash_function(key);

    switch (_typeHT) {
        case TypeHashTable::Sorted: {
            auto& chain = table[id];
        
            auto it = chain.begin();
            while (it != chain.end() && it->first < key) {
                ++it;
            }
            if (it != chain.end() && it->first == key) {
                return it->second;          
            }
            
            it = chain.insert(it, {key, ""});
            ++_filled;
            return it->second;
        }
        default: { // UnSorted
            for (auto& p : table[id]) {
                if (p.first == key) {
                    return p.second;
                }
            }

            table[id].push_front({key, ""});
            ++_filled;
            return table[id].front().second;
        }
    }
}


double HashTable::getLoadFactor() const {
    if (_capacity == 0) {
        return 0.0;
    }
    
    return static_cast<double>(_filled)/static_cast<double>(_capacity);
}

void HashTable::resize() noexcept {
    size_t new_capacity = _capacity * 2;
    std::vector<std::list<std::pair<int, std::string>>> newTable(new_capacity);

    for (size_t i = 0; i < table.size(); ++i) {
        for (const auto& pair : table[i]) {
            size_t new_index = hash_function(pair.first); 
            newTable[new_index].push_front(pair);
        }
    }

    table = std::move(newTable);
    _capacity = new_capacity; 
}

void HashTable::clear() {
    table.clear();
    _filled = 0;
}

void HashTable::print() const {
    for (size_t i = 0; i < table.size(); ++i) {
        if (!table[i].empty()) {
            std::cout << "Bucket " << i << ": ";
            for (const auto& pair : table[i]) {
                std::cout << "(" << pair.first << ", " << pair.second << ") ";
            }
            std::cout << std::endl;
        }
    }
}

size_t HashTable::hash_function(const int key) const {
   /* uint32_t hash = static_cast<uint32_t>(key);
    hash ^= (hash >> 10) ^ (hash >> 6);

    return (hash ^ (hash >> 4) ^ (hash >> 2)) % _capacity;*/
    size_t h = static_cast<size_t>(key);
    h = (h ^ 61) ^ (h >> 16);
    h = h + (h << 3);
    h = h ^ (h >> 4);
    h = h * 0x27d4eb2d;
    h = h ^ (h >> 15);
    return h % _capacity;
}

