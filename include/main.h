//
// Created by y1726 on 24-7-9.
//

#ifndef MAIN_H
#define MAIN_H
#include <winsock2.h>
#include <Windows.h>
#include <QButtonGroup>
#include <QCoreApplication>
#include <QObject>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <semaphore>
#include <random>
#include <ranges>
#include <atomic>
#include <thread>
#include <QAbstractNativeEventFilter>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QMessageBox>
#include <QTableWidget>
#include <QTimer>
#include <QStyleFactory>
#include <iostream>
#include <QFile>
#include <QResource>
#include <QStandardPaths>
#include <QSettings>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <QCloseEvent>
#include <QLineEdit>
#include <QFileDevice>
#include <QSharedMemory>
#include <QMainWindow>
#include <QWindow>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include <QThread>
#include <QLocalSocket>
#include <regex>


#include "resources/images.h"


inline double FACTOR = 1.00;
inline int DELAY  = 1150;
inline int DELAY_MAGNIFICATION  = 5;
// 副本悬赏接取同步机制
inline std::array BountyAccess = {true, true, true, true, true, true, true, true, true, true};
inline std::array BountyFinish = {true, true, true, true, true, true, true, true, true, true};
// 江湖行商同步机制
inline std::array MerchantLakeFinish = {false, false, false, false, false, false, false, false, false, false};
// 聚义平冤同步机制
inline std::array PacifyFinish = {false, false, false, false, false, false, false, false, false, false};

#endif //MAIN_H
