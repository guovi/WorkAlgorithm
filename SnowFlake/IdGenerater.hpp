#ifndef ID_GENERATER_H
#define ID_GENERATER_H

#include <string>
#include <mutex>
#include <chrono>
#include <cstdint>
#include <iostream>

namespace id_generater {


constexpr int UNUSED_BITS = 1;
constexpr int EPOCH_BITS = 41;
constexpr int NODE_ID_BITS = 10;
constexpr int SEQUENCE_BITS = 12;

constexpr std::int64_t MAX_NODE_ID = (1ull << NODE_ID_BITS) - 1;
constexpr std::int64_t MAX_SEQUENCE = (1ull << SEQUENCE_BITS) - 1;

constexpr std::int64_t START_POINT = 1714492800000ull;

class Id_Generater final {
public:
    static Id_Generater* get_instance(int t_node_id = 0, int t_sequence_id = 0) {
        std::cout << "begining...." << std::endl;
        if (t_node_id < 0 || t_node_id > MAX_NODE_ID) {
            throw std::invalid_argument(std::string("NODE_ID不应小于0或大于") + std::to_string(MAX_NODE_ID));
        }
        // if (t_sequence_id < 0 || t_sequence_id > MAX_SEQUENCE) {
        //     throw std::invalid_argument(std::string("iDataCenterID不应小于0或大于") + std::to_string(MAX_SEQUENCE));
        // }
        static Id_Generater ins(t_node_id, t_sequence_id);
        return &ins;
    }

    std::int64_t next_id() {
        std::lock_guard<std::mutex> lg(_mtx);
        auto cur_time_stamp = get_time_stamp();
        if (cur_time_stamp < _last_time_stamp) {
            throw std::invalid_argument(std::string("invalid system clock!"));
        } else if (cur_time_stamp == _last_time_stamp) {
            _sequence_id = (_sequence_id + 1) & MAX_SEQUENCE;
            if (_sequence_id == 0) {
                cur_time_stamp = wait_next_time_stamp(cur_time_stamp);
            }
        } else {
            _sequence_id = 0ull;
        }

        _last_time_stamp = cur_time_stamp;
        return (cur_time_stamp - START_POINT) << (NODE_ID_BITS + SEQUENCE_BITS) 
               | (_node_id << SEQUENCE_BITS)
               | _sequence_id;
    }

private:
    Id_Generater(int t_node_id, int t_sequence_id): _node_id(t_node_id), _sequence_id(t_sequence_id), _last_time_stamp(0) {

    }

    std::int64_t get_time_stamp() {
        auto time_point = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());    //获取时间并降低精度到毫秒
        return time_point.time_since_epoch().count();                                                                    //得到时间戳
    }

    std::int64_t wait_next_time_stamp(std::int64_t cur_time_stamp) {
        while (cur_time_stamp <= _last_time_stamp) {
            cur_time_stamp = get_time_stamp();
        }
        return cur_time_stamp;
    }



    std::int64_t _node_id;
    std::int64_t _sequence_id;
    std::int64_t _last_time_stamp;
    std::mutex _mtx;

}; // Id_Generater


}  // namespace id_generater

#endif  // ID_GENERATER_H
