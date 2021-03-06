#include <map>
#include "catch.hpp"
#include "art/radix_map.h"


SCENARIO("given an empty container", "[iterator]") {
    art::radix_map<unsigned, long> map;

    THEN ("iterator's begin is equal to its end") {
        REQUIRE(map.begin() == map.end());
    }
    THEN ("iterator's reverse begin is equal to its reverse end") {
        REQUIRE(map.rbegin() == map.rend());
    }
    WHEN("one element is inserted") {
        map.emplace(1, 5);
        THEN("begin can be dereferenced") {
            REQUIRE(map.begin()->first == 1);
            REQUIRE(map.begin()->second == 5);
        }
        THEN("rbegin can be dereferenced") {
            REQUIRE(map.rbegin()->first == 1);
            REQUIRE(map.rbegin()->second == 5);
        }
    }
}

SCENARIO("iteration with unsigned integers compared to std::map", "[iterator]") {
    art::radix_map<unsigned, unsigned> radix_map;
    std::map<unsigned, unsigned> std_set;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> dis(0, 1000000);

    for (unsigned i = 0; i < 10000; i++) {
        auto k = dis(gen);
        radix_map.insert(std::pair<unsigned, unsigned>(k, i));
        std_set.emplace(k, i);
    }

    REQUIRE(radix_map.begin()->second == std_set.begin()->second);
    REQUIRE(radix_map.rbegin()->second == std_set.rbegin()->second);

    THEN ("forward iteration is equal to std::map") {
        auto it_radix = radix_map.begin(), it_radix_end = radix_map.end();
        auto it_std = std_set.begin(), it_std_end = std_set.end();
        for (; it_std != it_std_end; ++it_radix, ++it_std) {
            REQUIRE(it_radix->second == it_std->second);
        }
        REQUIRE(it_radix == it_radix_end);
    }
    THEN ("reverse iteration is equal to std::map") {
        auto it_radix_reverse = radix_map.rbegin(), it_radix_rend = radix_map.rend();
        auto it_std_reverse = std_set.rbegin(), it_std_rend = std_set.rend();
        for (; it_std_reverse != it_std_rend; ++it_radix_reverse, ++it_std_reverse) {
            REQUIRE(it_radix_reverse->second == it_std_reverse->second);
        }
        REQUIRE(it_radix_reverse == it_radix_rend);
    }
}

SCENARIO("given an art with signed integer key", "[iterator]") {
    art::radix_map<int, int> map;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1000000, 1000000);

    std::vector<int> data;
    for (int i = 0; i < 10000; i++) {
        auto candidate = dis(gen);
        data.push_back(candidate);
    }

    for (auto &d : data)
        map.insert(std::make_pair(d, d));

    THEN("forward iteration is sorted correctly") {
        REQUIRE(std::is_sorted(map.begin(), map.end()));
    }

    THEN("reverse iteration is sorted correctly") {
        REQUIRE(std::is_sorted(map.rbegin(), map.rend(),
                               [](std::pair<int, int> a, std::pair<int, int> b) {
                                   return std::tie(a.first, a.second) > std::tie(b.first, b.second);
                               })
        );
    }

}

SCENARIO("given an art with unsigned integer key", "[iterator]") {
    art::radix_map<unsigned, unsigned> radix_map;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1000000);

    std::vector<unsigned> data;
    for (int i = 0; i < 10000; i++) {
        auto candidate = (unsigned) dis(gen);
        data.push_back(candidate);
    }

    for (auto &d : data)
        radix_map.insert(std::make_pair(d, d));

    THEN("forward iteration is sorted correctly") {
        REQUIRE(std::is_sorted(radix_map.begin(), radix_map.end()));
    }
    THEN("reverse iteration is sorted correctly") {
        REQUIRE(std::is_sorted(radix_map.rbegin(), radix_map.rend(),
                               [](std::pair<unsigned, unsigned> a, std::pair<unsigned, unsigned> b) {
                                   return std::tie(a.first, a.second) > std::tie(b.first, b.second);
                               })
        );
    }
}