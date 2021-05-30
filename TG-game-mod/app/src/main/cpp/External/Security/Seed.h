#pragma once

#define MRANDI32_MIN INT32_MIN
#define MRANDI32_MAX INT32_MAX

#define MRANDI64_MIN INT64_MIN
#define MRANDI64_MAX INT64_MAX 

#define MRANDU32_MIN 0x38e38e38U
#define MRANDU32_MAX 0xffffffffU

#define MRANDU64_MIN 0x38e38e38e38e3800ULL // 4099276460824344576
#define MRANDU64_MAX 0xffffffffffffffffULL // 18446744073709551615

class Seed {
private:
    uint32_t seed;
public:
    Seed();

    Seed(uint32_t sd);

    operator uint32_t() const;

    // I32
    int32_t GenRandomI32();

    int32_t GenRandomI32(int32_t num);

    int32_t GenRandomI32(int32_t min, int32_t max);

    int32_t GenRandomI32(int32_t num, int32_t min, int32_t max);
    
    // U32
    uint32_t GenRandomU32();

    uint32_t GenRandomU32(int32_t num);

    uint32_t GenRandomU32(uint32_t min, uint32_t max);

    uint32_t GenRandomU32(int32_t num, uint32_t min, uint32_t max);

    // I64
    int64_t GenRandomI64();

    int64_t GenRandomI64(int32_t num);

    int64_t GenRandomI64(int64_t min, int64_t max);

    int64_t GenRandomI64(int32_t num, int64_t min, int64_t max);

    // U64
    uint64_t GenRandomU64();

    uint64_t GenRandomU64(int32_t num);

    uint64_t GenRandomU64(uint64_t min, uint64_t max);

    uint64_t GenRandomU64(int32_t num, uint64_t min, uint64_t max);
};
