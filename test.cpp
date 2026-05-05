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
#include <cmath>

TEST_CASE("HashTable basic operations", "[unit]") {
    HashTable ht(100);   
    int32_t capacity = 0;

    SECTION("Insert and find") {
        ht.insert(42, "answer");
        ++capacity;
        REQUIRE(ht.getLoadFactor() == 1.0/100.0);
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
        ++capacity;
        ht.remove(1);
        std::string value;
        --capacity;
        REQUIRE(ht.getLoadFactor() == capacity/100.0);
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


const size_t N_MAX = 1000000;          // максимальное количество элементов
const size_t STEP = 2500;             // шаг увеличения N
const size_t FIXED_CAPACITY = N_MAX;   // фикс. число корзин


std::vector<int> generate_keys(size_t count) {
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }
    std::vector<int> keys;
    keys.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        keys.push_back(1 + (std::rand() % (count * 10)));
    }
    return keys;
}


double measure_insert_sorted(size_t N) {

    HashTable ht(FIXED_CAPACITY, TypeHashTable::Sorted);
    auto keys = generate_keys(N);
    auto start = std::chrono::high_resolution_clock::now();
    for (int k : keys) {
        ht.insert(k, std::to_string(k));
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double total = elapsed.count() / N; //среднее время одной операции
    
    return total;
}

double measure_insert_unsorted(size_t N) {
    
    HashTable ht(FIXED_CAPACITY, TypeHashTable::UnSorted);
    auto keys = generate_keys(N);
    auto start = std::chrono::high_resolution_clock::now();
    for (int k : keys) {
        ht.insert(k, std::to_string(k));
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double total = elapsed.count() / N; //среднее время одной операции
    
    return total;
}


double measure_find_sorted(size_t N) {
    
    HashTable ht(FIXED_CAPACITY, TypeHashTable::Sorted);
    auto keys = generate_keys(N); 
    for (int k : keys) {
        ht.insert(k, std::to_string(k));
    }
    auto start = std::chrono::high_resolution_clock::now();
    for (int k : keys) {
        std::string value;
        bool found = ht.find(k, value);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double total = elapsed.count() / N; //среднее время одной операции
    
    return total;
}

double measure_find_unsorted(size_t N) {

    HashTable ht(FIXED_CAPACITY, TypeHashTable::UnSorted);
    auto keys = generate_keys(N);
    for (int k : keys) {
        ht.insert(k, std::to_string(k));
    }
    auto start = std::chrono::high_resolution_clock::now();
    for (int k : keys) {
        std::string value;
        bool found = ht.find(k, value);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double total = elapsed.count() / N; //среднее время одной операции
    
    return total;
}

TEST_CASE("insertion performance (sorted vs unsorted)", "[insert_benchmark]") {
    std::vector<size_t> sizes;
    std::vector<double> insert_unsorted, insert_sorted;

    for (size_t N = 10000; N <= 100000; N += STEP) {
        sizes.push_back(N);
        double unSortedTime = measure_insert_unsorted(N);
        double sortedTime  = measure_insert_sorted(N);
        insert_unsorted.push_back(unSortedTime);
        insert_sorted.push_back(sortedTime);

        std::cout << "N = " << N
                  << " | Insert unsorted: " << unSortedTime * 1e6 << " microseconds"
                  << " | Insert sorted:   " << sortedTime * 1e6 << " microseconds\n";
    }

    std::ofstream file("insert_comparison.csv");
    file << "N,InsertUnsorted_sec,InsertSorted_sec\n";
    for (size_t i = 0; i < sizes.size(); ++i) {
        file << sizes[i] << ',' << insert_unsorted[i] << ',' << insert_sorted[i] << '\n';
    }
    std::cout << "Insert results saved to insert_comparison.csv\n";
}

TEST_CASE("find performance (sorted vs unsorted)", "[find_benchmark]") {
    std::vector<size_t> sizes;
    std::vector<double> find_unsorted, find_sorted;

    for (size_t N = 50000; N <= 100000; N += STEP) {
        sizes.push_back(N);
        double unSortedTime = measure_find_unsorted(N);
        double sortedTime  = measure_find_sorted(N);
        find_unsorted.push_back(unSortedTime);
        find_sorted.push_back(sortedTime);

        std::cout << "N = " << N
                  << " | Find unsorted: " << unSortedTime * 1e6 << " microseconds"
                  << " | Find sorted:   " << sortedTime * 1e6 << " microseconds\n";
    }

    std::ofstream file("find_comparison.csv");
    file << "N,FindUnsorted_sec,FindSorted_sec\n";
    for (size_t i = 0; i < sizes.size(); ++i) {
        file << sizes[i] << ',' << find_unsorted[i] << ',' << find_sorted[i] << '\n';
    }
    std::cout << "Find results saved to find_comparison.csv\n";
}