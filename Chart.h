#ifndef LAB3_CHART_H
#define LAB3_CHART_H


#include "chartDrawer.h"
#include "dataReader.h"
#include "IOCContainer.h"

// Класс для работы с диаграммой
class Chart {
public:
    Chart() = default;
    ~Chart() = default;

    QChartView& getView(); // Получение представления графика
    void changeColor();    // Изменение цвета

    void updateData(const QString& path); // Обновление данных диаграммы по пути к файлу
    void updateChart();                   // Обновление диаграммы
private:
    // Поля
    QChartView view;  // Представление
    DataList data{};  // Данные
    bool color = true;// Поле, определяющие цвет графика
    // true - цветной, false - черно-белый
};


#endif //LAB3_CHART_H