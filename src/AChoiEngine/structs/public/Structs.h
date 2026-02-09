//#pragma once
//
//struct PairHash {
//    size_t operator()(const std::pair<int, int>& p) const noexcept {
//        return std::hash<int>{}(p.first) ^ (std::hash<int>{}(p.second) << 1);
//    }
//};
//struct Array3Hash {
//    size_t operator()(const std::array<int, 3>& a) const noexcept {
//        size_t h = 0;
//        // simple combine
//        h ^= std::hash<int>{}(a[0]) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
//        h ^= std::hash<int>{}(a[1]) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
//        h ^= std::hash<int>{}(a[2]) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
//        return h;
//    }
//};

