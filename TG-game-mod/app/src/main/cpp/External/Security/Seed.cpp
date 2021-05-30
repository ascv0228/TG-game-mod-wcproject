#include <random>
#include <iostream>

#include "Seed.h"
#include "Distribution.hpp"

Seed::Seed(){
    seed = std::random_device()();
}

Seed::Seed(uint32_t sd) {
    seed = sd;
}

Seed::operator uint32_t() const {
    return seed;
}

int32_t Seed::GenRandomI32() {
    return GenRandomI32(1);
}

int32_t Seed::GenRandomI32(int32_t num) {
    return GenRandomI32(num, MRANDI32_MIN, MRANDI32_MAX);
}

int32_t Seed::GenRandomI32(int32_t min, int32_t max) {
    return GenRandomI32(1, min, max);
}

int32_t Seed::GenRandomI32(int32_t num, int32_t min, int32_t max) {
    std::mt19937 generator { seed };

    int32_t rand = 0;
    for (int32_t i = 0; i < num; i++) {
        rand = my_uniform_int_distribution<int32_t>(min, max)(generator);
    }
    return rand;
}

uint32_t Seed::GenRandomU32() {
    return GenRandomU32(1);
}

uint32_t Seed::GenRandomU32(int32_t num) {
    return GenRandomU32(num, MRANDU32_MIN, MRANDU32_MAX);
}

uint32_t Seed::GenRandomU32(uint32_t min, uint32_t max) {
    return GenRandomU32(1, min, max);
}

uint32_t Seed::GenRandomU32(int32_t num, uint32_t min, uint32_t max) {
    std::mt19937 generator { seed };

    uint32_t rand = 0;
    for (int32_t i = 0; i < num; i++) {
        rand = my_uniform_int_distribution<uint32_t>(min, max)(generator);
    }
    return rand;
}

int64_t Seed::GenRandomI64() {
    return GenRandomI64(1);
}

int64_t Seed::GenRandomI64(int32_t num) {
    return GenRandomI64(num, MRANDI64_MIN, MRANDI64_MAX);
}

int64_t Seed::GenRandomI64(int64_t min, int64_t max) {
    return GenRandomI64(1, min, max);
}

int64_t Seed::GenRandomI64(int32_t num, int64_t min, int64_t max) {
    std::mt19937_64 generator { seed };

    int64_t rand = 0;
    for (int32_t i = 0; i < num; i++) {
        rand = my_uniform_int_distribution<int64_t>(min, max)(generator);
    }
    return rand;
}



uint64_t Seed::GenRandomU64() {
    return GenRandomU64(1);
}

uint64_t Seed::GenRandomU64(int32_t num) {
    return GenRandomU64(num, MRANDU64_MIN, MRANDU64_MAX);
}

uint64_t Seed::GenRandomU64(uint64_t min, uint64_t max) {
    return GenRandomU64(1, min, max);
}

uint64_t Seed::GenRandomU64(int32_t num, uint64_t min, uint64_t max) {
    std::mt19937_64 generator { seed };

    uint64_t rand = 0;
    for (int32_t i = 0; i < num; i++) {
        rand = my_uniform_int_distribution<uint64_t>(min, max)(generator);
    }
    return rand;
}

