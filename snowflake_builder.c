#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

#ifndef SEQUENCE_BITS
#define SEQUENCE_BITS 12 // 序列号占用位数
#endif

#ifndef MACHINE_ID_BITS
#define MACHINE_ID_BITS 5   // 机器 ID 占用位数
#endif

#ifndef DATA_CENTER_ID_BITS
#define DATA_CENTER_ID_BITS 5   // 数据中心 ID 占用位数
#endif

typedef uint64_t snowflake_id_t;

// 时间戳起始时间，可以根据实际需求进行调整
const uint64_t EPOCH = 1616000000000;

// 数据中心 ID，可根据实际需求进行调整
const uint64_t DATA_CENTER_ID = 0;

// 机器 ID，可根据实际需求进行调整
const uint64_t MACHINE_ID = 0;

// 序列号掩码
const uint64_t SEQUENCE_MASK = (1 << SEQUENCE_BITS) - 1;

// 时间戳左移位数
const uint64_t TIMESTAMP_LEFT_SHIFT = SEQUENCE_BITS + MACHINE_ID_BITS + DATA_CENTER_ID_BITS;

// 数据中心 ID 左移位数
const uint64_t DATA_CENTER_ID_LEFT_SHIFT = SEQUENCE_BITS + MACHINE_ID_BITS;

// 机器 ID 左移位数
const uint64_t MACHINE_ID_LEFT_SHIFT = SEQUENCE_BITS;

// 上次生成的序列号
static uint64_t last_sequence = 0;

// 上次生成的时间戳
static uint64_t last_timestamp = 0;

// 序列号
static uint64_t sequence = 0;

// 获取当前时间戳（毫秒级）
static uint64_t get_timestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

// 生成 snowflake ID
snowflake_id_t generate_id() {
    uint64_t timestamp = get_timestamp();

    // 如果当前时间小于上次生成 ID 的时间戳，则说明系统时钟出现了回退，需要等待时钟追上来
    if (timestamp < last_timestamp) {
        timestamp = last_timestamp;
    }

    // 如果是同一毫秒内生成的，则自增序列号
    if (timestamp == last_timestamp) {
        sequence = (sequence + 1) & SEQUENCE_MASK;
        if (sequence == 0) {
            // 同一毫秒内序列号已经用完，等待下一毫秒
            timestamp = get_timestamp();
            while (timestamp <= last_timestamp) {
                timestamp = get_timestamp();
            }
        }
    } else {
        // 不是同一毫秒内生成的，序列号重置为0
        sequence = 0;
    }

    last_sequence = sequence;
    last_timestamp = timestamp;

    // 构造 snowflake ID
    snowflake_id_t id = ((timestamp - EPOCH) << TIMESTAMP_LEFT_SHIFT) |
                        (DATA_CENTER_ID << DATA_CENTER_ID_LEFT_SHIFT) |
                        (MACHINE_ID << MACHINE_ID_LEFT_SHIFT) |
                        sequence;
    return id;
}
