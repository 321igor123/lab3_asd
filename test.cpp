#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "HashTable.hpp"
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <algorithm>   


const size_t FIXED_CAPACITY = 1024;   
const size_t STEP          = 500;      // шаг увеличения N
const size_t MAX_N         = 30000;    // максимальный размер набора данных
const int    REPETITIONS   = 10;       // число повторных замеров для усреднения


TEST_CASE("HashTable basic operations", "[unit]") {
    HashTable ht(100);

    SECTION("Insert and find") {
        ht.insert(42, "answer");
        REQUIRE(ht.getLoadFactor() == 1.0 / 100.0);
        std::string value;
        REQUIRE(ht.find(42, value));
        REQUIRE(value == "answer");
    }

    SECTION("Find non-existing key") {
        std::string value;
        REQUIRE(!ht.find(999, value));
    }

    SECTION("Remove existing key") {
        ht.insert(1, "one");
        ht.remove(1);
        std::string value;
        REQUIRE(ht.getLoadFactor() == 0.0);   
        REQUIRE(!ht.find(1, value));
    }

    SECTION("Operator[] access – read and write") {
        ht.insert(10, "ten");
        REQUIRE(ht[10] == "ten");
        ht[10] = "diez";
        REQUIRE(ht[10] == "diez");
        ht[20] = "twenty";
        std::string val;
        REQUIRE(ht.find(20, val));
        REQUIRE(val == "twenty");
    }
}


std::vector<int> generate_keys(size_t count) {
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }

    std::unordered_set<int> unique_keys;
    unique_keys.reserve(count);
    while (unique_keys.size() < count) {
        int key = 1 + (std::rand() % (count * 10));
        unique_keys.insert(key);
    }
    return std::vector<int>(unique_keys.begin(), unique_keys.end());
}


double measure_insert(TypeHashTable type, size_t N, const std::vector<int>& keys) {
    double total_time = 0.0;
    for (int rep = 0; rep < REPETITIONS; ++rep) {
        HashTable ht(FIXED_CAPACITY, type);
        auto start = std::chrono::high_resolution_clock::now();
        for (int k : keys) {
            ht.insert(k, "value");
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        total_time += elapsed.count();
    }
   
    return total_time / (N * REPETITIONS);
}

// Измеряет среднее время поиска ОДНОГО элемента (в секундах) для заданного N и типа таблицы
double measure_find(TypeHashTable type, size_t N, const std::vector<int>& keys) {
    double total_time = 0.0;
    for (int rep = 0; rep < REPETITIONS; ++rep) {
        HashTable ht(FIXED_CAPACITY, type);
        
        for (int k : keys) {
            ht.insert(k, "value");
        }
        auto start = std::chrono::high_resolution_clock::now();
        for (int k : keys) {
            std::string value;
            bool found = ht.find(k, value);
            (void)found; 
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        total_time += elapsed.count();
    }
    return total_time / (N * REPETITIONS);
}

TEST_CASE("Insert performance (sorted vs unsorted)", "[insert_benchmark]") {
    std::vector<size_t> sizes;
    std::vector<double> insert_unsorted, insert_sorted;


    for (size_t N = 1000; N <= MAX_N; N += STEP) {
        auto keys = generate_keys(N);
        sizes.push_back(N);

        double unSortedTime = measure_insert(TypeHashTable::UnSorted, N, keys);
        double sortedTime   = measure_insert(TypeHashTable::Sorted,   N, keys);

        insert_unsorted.push_back(unSortedTime);
        insert_sorted.push_back(sortedTime);

        std::cout << "N = " << N
                  << " | Insert unsorted: " << unSortedTime * 1e6 << " µs"
                  << " | Insert sorted:   " << sortedTime * 1e6 << " µs\n";
    }

    std::ofstream file("insert_comparison.csv");
    file << "N,InsertUnsorted_sec,InsertSorted_sec\n";
    for (size_t i = 0; i < sizes.size(); ++i) {
        file << sizes[i] << ',' << insert_unsorted[i] << ',' << insert_sorted[i] << '\n';
    }
    std::cout << "Insert results saved to insert_comparison.csv\n";
}

TEST_CASE("Find performance (sorted vs unsorted)", "[find_benchmark]") {
    std::vector<size_t> sizes;
    std::vector<double> find_unsorted, find_sorted;

    for (size_t N = 1000; N <= MAX_N; N += STEP) {
        auto keys = generate_keys(N);
        sizes.push_back(N);

        double unSortedTime = measure_find(TypeHashTable::UnSorted, N, keys);
        double sortedTime   = measure_find(TypeHashTable::Sorted,   N, keys);

        find_unsorted.push_back(unSortedTime);
        find_sorted.push_back(sortedTime);

        std::cout << "N = " << N
                  << " | Find unsorted: " << unSortedTime * 1e6 << " µs"
                  << " | Find sorted:   " << sortedTime * 1e6 << " µs\n";
    }

    std::ofstream file("find_comparison.csv");
    file << "N,FindUnsorted_sec,FindSorted_sec\n";
    for (size_t i = 0; i < sizes.size(); ++i) {
        file << sizes[i] << ',' << find_unsorted[i] << ',' << find_sorted[i] << '\n';
    }
    std::cout << "Find results saved to find_comparison.csv\n";
}