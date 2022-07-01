#include "Chart.h"

// Определяем статическое поля нашего IOCContainera
int IOCContainer::s_typeId = 0;

// Получение представления
QChartView& Chart::getView() {
    return view;
}
// Изменение цвета
void Chart::changeColor() {
    color = !color;
}
// Обновление данных по пути к файлу
void Chart::updateData(const QString& path) {
    // Перезаписываем данных обращаясь к IOC контейнеру
    IOCContainer::instance().GetObject<IdataReader>()->readData(path, data);
}
// Обновление диаграммы
void Chart::updateChart() {
    // Перерисовываем диаграмму обращаясь к IOC контейнеру
    IOCContainer::instance().GetObject<IchartDrawer>()->drawChart(data, view, color);
}