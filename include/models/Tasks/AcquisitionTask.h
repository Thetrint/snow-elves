//
// Created by y1726 on 24-8-15.
//

#ifndef ACQUISITIONTASK_H
#define ACQUISITIONTASK_H
#include "models/Tasks/BasicTask.h"

class AcquisitionTask final : public BasicTask{

protected:
    int id; // 任务ID
    HWND hwnd; //
    std::mutex& pause_event;
    bool& unbind_event;
    std::ifstream& ifs;

public:
    AcquisitionTask(const int id, const HWND hwnd, std::mutex& pause_event, bool& unbind_event, bool& disrupted, std::ifstream& ifs) : BasicTask(id, hwnd, pause_event, unbind_event, disrupted, ifs), id(id), hwnd(hwnd), pause_event(pause_event), unbind_event(unbind_event), ifs(ifs) {}

    // 实现基类的纯虚函数
    int implementation() override;

    void objective(std::string ve) override;

    int determine() override;

    int detect() override;

    void hasten();

private:
    struct Location {
        std::string x;
        std::string y;
    };

    std::map<std::string, std::string> SetText{
        {"枯木_风雷岛", "$风雷岛$  570#463  562#462"},
        {"翠竹_金陵", "$金陵$  804#764"},
        {"榆木_江南", "$江南$  1686#969 1603#873"},
        {"枫木_江南", "$江南$  1054#538 1097#486 1139#439"},
        {"松木_江南", "$江南$  1099#680 1098#610 1099#581 1089#562 1044#581 998#495 970#467 1002#449 946#549 1266#756 1296#744 1345#788 1335#795 1354#710 1370#697"},
        {"桉木_江南", "$江南$  928#745 1732#601 1550#901"},
        {"野花_江南", "$江南$  1756#981"},
        {"杂草_江南", "$江南$  1764#966"},
        {"朱果_江南", "$江南$  970#1672 967#1665 951#1653 963#1607 970#1604 995#1573 987#1571 953#1582 935#1544 933#1540 928#1526 949#1529"},
        {"地灵果_少林", "$少林$  222#553 193#531 186#544 189#530 163#458 156#452 133#438 57#445 267#514 255#452 278#447 281#441 288#457 291#453"},
        {"灵芝_华山", "$华山$  169#62  228#72 66#40 296#169 361#131 342#146 363#96 378#80 0#348"},
        {"碎石_中原", "$中原$  1055#1738"},
        {"黄铜_中原", "$中原$  924#1689"},
        {"立银_中原", "$中原$  984#1640  978#1622 965#1615 1284#1537"},
        {"金矿_塞北", "$塞北$  1552#1786  1526#1803"},
        {"祖母绿_塞北", "$塞北$  1579#1788 1575#1802 1565#1790"},
        {"钨晶_塞北", "$塞北$  1554#1824 1566#1824 1575#1829 1585#1841"},
    };
    std::jthread thread;
    std::vector<Location> LocationVector;
    /**
     * 0. 记录采集换线
     * 1. 记录采集次数
     */
    std::vector<int> record_num{1, 1};
    /**
     * 记录吃鸡蛋时间 3分钟内不重复吃鸡蛋
     */
    std::vector<std::chrono::steady_clock::time_point> record_time{std::chrono::steady_clock::time_point()};
    /**
     * 0. 采集加速标志
     */
    std::vector<bool> record_event{false};

};
#endif //ACQUISITIONTASK_H
