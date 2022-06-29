#ifndef LAB3_CHARTDRAWER_H
#define LAB3_CHARTDRAWER_H


#include <QtCharts>


using Data = QPair<QPointF, QString>;
using DataList = QList<Data>;


class IchartDrawer {
public:
    IchartDrawer() = default;
    virtual ~IchartDrawer() = default;

    virtual void drawChart(const DataList& data, QChartView& view, bool color) = 0;
};


class pieChartDrawer : public IchartDrawer {
public:
    pieChartDrawer() = default;

    virtual void drawChart(const DataList& data, QChartView& view, bool color) override;
};


class barChartDrawer : public IchartDrawer {
public:
    barChartDrawer() = default;

    virtual void drawChart(const DataList& data, QChartView& view, bool color) override;
};


#endif //LAB3_CHARTDRAWER_H