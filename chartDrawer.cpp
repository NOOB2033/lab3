#include "chartDrawer.h"


void pieChartDrawer::drawChart(const DataList& data, QChartView& view, bool color) {
    QChart* chart = new QChart;
    chart->setTitle("Pie chart");

    QPieSeries* series = new QPieSeries(chart);

    size_t count = 0;
    for (auto elem : data)
    {
        series->append(elem.second, elem.first.y());
        if (!color) {
            QLinearGradient gradient(0, 0, view.size().width(), view.size().height());
            gradient.setColorAt(0, Qt::black);
            gradient.setColorAt(1, Qt::white);

            QBrush brush(gradient);
            series->slices()[count]->setBrush(brush);
        }
        ++count;
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
    view.setChart(chart);
}


void barChartDrawer::drawChart(const DataList& data, QChartView& view, bool color) {
    QChart* chart = new QChart;
    chart->setTitle("Bar chart");

    QBarSeries* series = new QBarSeries(chart);

    size_t count = 0;
    for (auto elem : data)
    {
        QBarSet* set = new QBarSet(elem.second);
        if (!color) {
            QLinearGradient gradient(0, 0, view.size().width(), view.size().height());
            gradient.setColorAt(0, Qt::black);
            gradient.setColorAt(1, Qt::white);

            set->setBrush(QBrush(gradient));
        }
        *set << elem.first.y();
        series->append(set);
        ++count;
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
    view.setChart(chart);
}