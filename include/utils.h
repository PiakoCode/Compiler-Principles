#pragma once

#include <string>
#include <vector>

/**
 * @brief 
 * 
 * @param filepath 文件路径
 * @return std::string 文件内容
 */
std::string ReadFile(const std::string& filePath);

/**
 * @brief 
 * 将字符串进行拆分(不会保留定界符)
 * @param str 字符串
 * @param delimiter 定界符 
 * @return std::vector<std::string> 拆分后的字符串数组
 */
std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter);