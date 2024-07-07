//
// Created by y1726 on 2024/7/1.
//

#ifndef LOADJSONFILE_H
#define LOADJSONFILE_H
#include <QJsonObject>
#include <QJsonArray>

class LoadJsonFile {
public:
    static LoadJsonFile& instance();

    void LoadFile(int id);

    QJsonObject  file_0;
    QJsonObject  file_1;
    QJsonObject  file_2;
    QJsonObject  file_3;
    QJsonObject  file_4;
    QJsonObject  file_5;
    QJsonObject  file_6;
    QJsonObject  file_7;
    QJsonObject  file_8;
    QJsonObject  file_9;

private:

};
#endif //LOADJSONFILE_H

