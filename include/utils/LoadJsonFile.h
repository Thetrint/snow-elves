//
// Created by y1726 on 2024/7/1.
//

#ifndef LOADJSONFILE_H
#define LOADJSONFILE_H
#include <json/json.h>

class LoadJsonFile {
public:
    static LoadJsonFile& instance();

    void LoadFile(int id);

    Json::Value file_0;
    Json::Value file_1;
    Json::Value file_2;
    Json::Value file_3;
    Json::Value file_4;
    Json::Value file_5;
    Json::Value file_6;
    Json::Value file_7;
    Json::Value file_8;
    Json::Value file_9;

private:

};
#endif //LOADJSONFILE_H

