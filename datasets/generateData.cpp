#include <random>
#include <set>
#include <vector>
#include <fstream>

int main() {
    std::random_device rd;
    std::mt19937  engine(rd());

    std::uniform_int_distribution<long> dist(0, 1000000000L);
    std::set<long> key_set;
    std::vector<long> keys;
    while (key_set.size() < 1000) {
        long key = dist(engine);
        key_set.insert(key);
    }
    std::copy(key_set.begin(), key_set.end(), std::back_inserter(keys));
    std::fstream fileStream;
    fileStream.open("/home/jingtao8a/GRE/datasets/customData.txt", std::ios::out | std::ios::trunc);
    for (int i = 0; i < keys.size(); ++i) {
        fileStream << keys[i] << std::endl;
    }
    fileStream.close();
    return 0;
}