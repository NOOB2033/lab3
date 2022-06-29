#ifndef LAB3_CHART_H
#define LAB3_CHART_H


#include "chartDrawer.h"
#include "dataReader.h"
#include "IOCContainer.h"


class Chart {
public:
    Chart() = default;
    ~Chart() = default;

    QChartView& getView();
    void changeColor();

    void updateData(const QString& path);
    void updateChart();
private:
    QChartView view;
    DataList data{};
    bool color = true;
};


#endif //LAB3_CHART_H