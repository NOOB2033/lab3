#ifndef LAB3_CHARTDRAWER_H
#define LAB3_CHARTDRAWER_H


#include <QtCharts>


using Data = QPair<double, QString>; // Данные - пара<значение, строка(дата-время)>
using DataList = QList<Data>;        // Контейнер с данными

// Интерфейс для отправки данных из DataList в QChartView
class IchartDrawer {
public:
    IchartDrawer() = default;
    virtual ~IchartDrawer() = default;
    // Метод отправки данных из DataList в QChartView
    virtual void drawChart(const DataList& data, QChartView& view, bool color) = 0;
};

// Класс для отрисовки круговой диаграммы
class pieChartDrawer : public IchartDrawer {
public:
    pieChartDrawer() = default;

    virtual void drawChart(const DataList& data, QChartView& view, bool color) override;
};

// Класс для отрисовки вертикальной диаграммы
class barChartDrawer : public IchartDrawer {
public:
    barChartDrawer() = default;

    virtual void drawChart(const DataList& data, QChartView& view, bool color) override;
};


#endif //LAB3_CHARTDRAWER_H