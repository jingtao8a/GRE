//
// Created by jingtao8a on 3/24/24.
//


#include "../indexInterface.h"
#include <algorithm>
#define using_small_network
#include "./src/index/include/Controller.hpp"
#include "./src/index/include/Index.hpp"
#undef KEY_TYPE
#undef PAYLOAD_TYPE

template <class KEY_TYPE, class PAYLOAD_TYPE>
class ChameleonInterface : public indexInterface<KEY_TYPE, PAYLOAD_TYPE> {
public:
    void init(Param *param = nullptr) {
        std::cout << "ChameleonInterface::init" << std::endl;
        controller.load_in();//加载模型
    }

    void bulk_load(std::pair <KEY_TYPE, PAYLOAD_TYPE> *key_value, size_t num, Param *param = nullptr);

    bool get(KEY_TYPE key, PAYLOAD_TYPE &val, Param *param = nullptr);

    bool put(KEY_TYPE key, PAYLOAD_TYPE value, Param *param = nullptr);

    bool update(KEY_TYPE key, PAYLOAD_TYPE value, Param *param = nullptr);

    bool remove(KEY_TYPE key, Param *param = nullptr);

    size_t scan(KEY_TYPE key_low_bound, size_t key_num, std::pair <KEY_TYPE, PAYLOAD_TYPE> *result,
                Param *param = nullptr) { throw NotImplementedException(); }

    long long memory_consumption() { return (long long)index->memory_occupied(); }

private:
    GlobalController controller;
    experience_t exp_chosen;
    Hits::Configuration conf;
    Hits::Index<KEY_TYPE, VALUE_TYPE> *index;
};

template<class KEY_TYPE, class PAYLOAD_TYPE>
void ChameleonInterface<KEY_TYPE, PAYLOAD_TYPE>::bulk_load(std::pair <KEY_TYPE, PAYLOAD_TYPE> *key_value, size_t num, Param *param) {
    std::cout << "ChameleonInterface::bulk_load ......" << std::endl;
    std::vector<std::pair<KEY_TYPE, PAYLOAD_TYPE>> bulk_load_data;
    std::copy(key_value, key_value + num, std::back_inserter((bulk_load_data)));
    std::cout << "std::sort begin" << std::endl;
    std::sort(bulk_load_data.begin(), bulk_load_data.end(), [](const pair<keyType, recordPtr>& left, const pair<keyType, recordPtr>& right) {
        return left.first < right.first;
    });
    std::cout << "std::sort end" << std::endl;
    auto min_max = get_min_max<KEY_TYPE,VALUE_TYPE>(bulk_load_data.begin(),bulk_load_data.end());
    auto pdf = get_pdf<KEY_TYPE,VALUE_TYPE>(bulk_load_data.begin(),bulk_load_data.end(),min_max.first,min_max.second,PDF_SIZE);
    std::copy(pdf.begin(), pdf.end(), exp_chosen.distribution);
    exp_chosen.data_size = float(bulk_load_data.size());
    auto best_gen = controller.get_best_action_GA(exp_chosen);
    conf = best_gen.conf;
    index = new Hits::Index<KEY_TYPE, VALUE_TYPE>(conf,min_max.first, min_max.second);
    index->bulk_load(bulk_load_data.begin(), bulk_load_data.end());
    std::cout << "ChameleonInterface::bulk_load end" << std::endl;
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool ChameleonInterface<KEY_TYPE, PAYLOAD_TYPE>::get(KEY_TYPE key, PAYLOAD_TYPE &val, Param *param) {
    return index->get(key, val);
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool ChameleonInterface<KEY_TYPE, PAYLOAD_TYPE>::put(KEY_TYPE key, PAYLOAD_TYPE value, Param *param) {
    return index->add(key, value);
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool ChameleonInterface<KEY_TYPE, PAYLOAD_TYPE>::update(KEY_TYPE key, PAYLOAD_TYPE value, Param *param) {
    index->erase(key);
    return index->add(key, value);
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool ChameleonInterface<KEY_TYPE, PAYLOAD_TYPE>::remove(KEY_TYPE key, Param *param) {
    return index->erase(key);
}

