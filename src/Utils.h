#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sys/stat.h>
#include <map>
#include <chrono>
#include <algorithm>
#include <atomic>
#include <cassert>
#include <functional>
#include <thread>
#include <tuple>

namespace types {
    template<typename T> using Tuple = std::tuple<int, T, T>;
    template<typename T> using Table = std::vector<Tuple<T>>;
    template<typename T> using Database = std::vector<Table<T>>;    
}

namespace utils {
    // const bool isDirectory(const char *path) {
    //     struct stat info;
    //     if(stat(path, &info) != 0) return false;
    //     else if(info.st_mode & S_IFDIR) return true;
    //     else return false;
    // }

    // template <typename T>
    // void ReadTable(const std::string &path, int attributes, Table<T> &table) {
    //     std::ifstream input(path, std::ifstream::in);
    //     table.clear();
    //     Tuple<T> tuple(attributes);
    //     T elem;
    //     while (input >> elem){
    //         input >> elem_1;
    //         auto tuple = std::make_tuple(id, elem_0, elem_1);
    //         table.push_back(tuple);
    //         id++;
    //     }
    //     input.close();
    // }

    template<typename T>
    static std::vector<std::vector<T>> PartitionTable(const std::vector<T>& table, int n) {
        std::vector<std::vector<T>> parts;

        int length = table.size() / n;
        int k = table.size() % n;

        int begin = 0;
        int end = 0;

        int min = 0;
        n > table.size()? min = table.size() : min = n;

        for (int i = 0; i < min; ++i) {
            end += (k > 0) ? (length + !!(k--)) : length;
            parts.push_back(std::vector<T>(table.begin() + begin, table.begin() + end));
            begin = end;
        }
        return parts;
    }
}

#endif //UTILS_H