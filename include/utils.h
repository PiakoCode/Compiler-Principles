#pragma once
/**
 * @file utils.h
 * @brief 常用函数
 */

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <string_view>
#include <vector>

inline void Message(const std::string_view msg) { std::cout << msg; }

/**
 * @brief
 *
 * @param filepath 文件路径
 * @return std::string 文件内容
 */
std::string ReadFile(const std::string &filePath);

/**
 * @brief
 * 将字符串进行拆分(不会保留定界符)
 * @param str 字符串
 * @param delimiter 定界符
 * @return std::vector<std::string> 拆分后的字符串数组
 */
std::vector<std::string> SplitString(const std::string &str,
                                     const std::string &delimiter);

/**
 * @brief
 * 合并集合
 * @tparam T
 * @param a
 * @param b
 * @return std::set<T>
 */
template <typename T>
std::set<T> MergeSet(const std::set<T> &a, const std::set<T> &b) {
    std::set<T> c(a);
    c.insert(b.begin(), b.end());
    return c;
}

template <typename T>
bool IsInSet(T &a, const std::set<T> &b) {
    return b.find(a) != b.end();
}

template <typename T>
std::set<T> SetAdd(const std::set<T> &a, T &b) {
    std::set<T> c(a);
    c.insert(b);
    return c;
}
