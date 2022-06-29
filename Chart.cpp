#include "Chart.h"


int IOCContainer::s_typeId = 0;


QChartView& Chart::getView() {
    return view;
}

void Chart::changeColor() {
    color = !color;
}

void Chart::updateData(const QString& path) {
    IOCContainer::instance().GetObject<IdataReader>()->readData(path, data);
}

void Chart::updateChart() {
    IOCContainer::instance().GetObject<IchartDrawer>()->drawChart(data, view, color);
}