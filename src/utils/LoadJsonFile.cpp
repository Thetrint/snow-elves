//
// Created by y1726 on 2024/7/1.
//
#include "utils/LoadJsonFile.h"
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

// 定义 instance() 方法
LoadJsonFile& LoadJsonFile::instance() {
    static LoadJsonFile load_json_file;
    return load_json_file;
}


void LoadJsonFile::LoadFile(int id) {
    // 获取临时目录路径并设置文件路径
    std::filesystem::path tempDir = std::filesystem::temp_directory_path();
    std::filesystem::path filePath = tempDir / ("ElvesConfig_" + std::to_string(id) + ".json");

    std::ifstream inputFile(filePath);
    if (!inputFile.is_open())
    {
        std::cout << "Failed to open file: " << filePath << std::endl;

    }

    Json::CharReaderBuilder ReaderBuilder;
    ReaderBuilder["emitUTF8"] = true;
    std::string errs;

    Json::Value v;
    bool parsingSuccessful = Json::parseFromStream(ReaderBuilder, inputFile, &v, &errs);
    inputFile.close();

    if (!parsingSuccessful)
    {
        std::cout << "Failed to parse JSON: " << errs << std::endl;

    }
    switch (id) {
        case 0:
            file_0 = v;
            break;
        case 1:
            file_1 = v;
            break;
        case 2:
            file_2 = v;
            break;
        case 3:
            file_3 = v;
            break;
        case 4:
            file_4 = v;
            break;
        case 5:
            file_5 = v;
            break;
        case 6:
            file_6 = v;
            break;
        case 7:
            file_7 = v;
            break;
        case 8:
            file_8 = v;
            break;
        case 9:
            file_9 = v;
            break;
        default:
            break;
    }

}



