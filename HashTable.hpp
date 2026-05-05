#pragma once
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <cstdint>

enum class TypeHashTable
{
    Sorted,
    UnSorted
};

class HashTable 
{
public:
    explicit HashTable(size_t size = 100, TypeHashTable typeHT = TypeHashTable::UnSorted) noexcept;

    ~HashTable();
    // Если при вставке обнаруживается, что LoadFactor таблицы превышает 0.75, то таблица расширяется в 2 раза.
    void insert(const int key, const std::string& value);
    
    bool find(const int key, std::string& value) const;
    
    void remove(const int key);
    
    std::string& operator[](const int key);

    // Занятость хеш-таблицы. Считается как отношение количества занятых ячеек к вместимости хеш-таблицы.
    double getLoadFactor() const;

    void resize() noexcept;

    void clear();

    void print() const;

private:
    int32_t _capacity;  // Вместимость хеш-таблицы
    int32_t _filled;    // Текущее количество занятых ячеек хеш-таблицы

    TypeHashTable _typeHT;

    std::vector<std::list<std::pair<int, std::string>>> table;
    size_t hash_function(const int key) const; 
};