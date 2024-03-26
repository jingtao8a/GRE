//
// Created by jingtao8a on 3/24/24.
//

#include "./src/src/dili/DILI.h"
#include "../indexInterface.h"
#include "./src/src/utils/file_utils.h"
#include <algorithm>

#undef KEY_TYPE
#undef PAYLOAD_TYPE

class NotImplementedException : public std::exception {
public:
    virtual char const* what() { return "Function not yet implementd."; }
};

template <class KEY_TYPE, class PAYLOAD_TYPE>
class DILIInterface : public indexInterface<KEY_TYPE, PAYLOAD_TYPE> {
public:
    void init(Param *param = nullptr) {
        std::cout << "DILIInterface::init" << std::endl;
        string mirror_dir = "data/buTree";
        int status = file_utils::path_status(mirror_dir);
        assert(status != 2);
        if (status == 0) {
            file_utils::detect_and_create_dir(mirror_dir);
        }
        dili.set_mirror_dir(mirror_dir);
    }

    void bulk_load(std::pair <KEY_TYPE, PAYLOAD_TYPE> *key_value, size_t num, Param *param = nullptr);

    bool get(KEY_TYPE key, PAYLOAD_TYPE &val, Param *param = nullptr);

    bool put(KEY_TYPE key, PAYLOAD_TYPE value, Param *param = nullptr);

    bool update(KEY_TYPE key, PAYLOAD_TYPE value, Param *param = nullptr);

    bool remove(KEY_TYPE key, Param *param = nullptr);

    size_t scan(KEY_TYPE key_low_bound, size_t key_num, std::pair <KEY_TYPE, PAYLOAD_TYPE> *result,
                Param *param = nullptr) { throw NotImplementedException(); }

    long long memory_consumption() { return dili.size(); }
private:
    DILI dili;
};

template<class KEY_TYPE, class PAYLOAD_TYPE>
void DILIInterface<KEY_TYPE, PAYLOAD_TYPE>::bulk_load(std::pair <KEY_TYPE, PAYLOAD_TYPE> *key_value, size_t num, Param *param) {
    std::cout << "DILIInterface::bulk_load" << std::endl;
    std::vector<pair<keyType, recordPtr>> bulk_load_data;
    bulk_load_data.resize(num);
    //这里将recordPtr 与 PAYLOAD_TYPE 等价
    for (long i = 0; i < num; ++i) {
        bulk_load_data[i] = key_value[i];
    }
    std::cout << "std::sort begin" << std::endl;
    std::sort(bulk_load_data.begin(), bulk_load_data.end(), [](const pair<keyType, recordPtr>& left, const pair<keyType, recordPtr>& right) {
        return left.first < right.first;
    });
    std::cout << "std::sort end" << std::endl;
    dili.bulk_load(bulk_load_data);
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool DILIInterface<KEY_TYPE, PAYLOAD_TYPE>::get(KEY_TYPE key, PAYLOAD_TYPE &val, Param *param) {
    //这里将recordPtr 与 PAYLOAD_TYPE 等价
    recordPtr ptr = dili.search(key);
    if (ptr != -1) {
        val = ptr;
        return true;
    }
    return false;
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool DILIInterface<KEY_TYPE, PAYLOAD_TYPE>::put(KEY_TYPE key, PAYLOAD_TYPE value, Param *param) {
    return dili.insert(key, value);
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool DILIInterface<KEY_TYPE, PAYLOAD_TYPE>::update(KEY_TYPE key, PAYLOAD_TYPE value, Param *param) {
    remove(key, param);
    return put(key, value, param);
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool DILIInterface<KEY_TYPE, PAYLOAD_TYPE>::remove(KEY_TYPE key, Param *param) {
    recordPtr ptr = dili.delete_key(key);
    return ptr != -1;
}

