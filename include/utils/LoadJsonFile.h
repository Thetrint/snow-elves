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
    std::map<int, QJsonObject> jsonFiles;

private:

};
#endif //LOADJSONFILE_H

