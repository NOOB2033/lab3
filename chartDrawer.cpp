#include "chartDrawer.h"

// Отрисовка круговой диаграммы
void pieChartDrawer::drawChart(const DataList& data, QChartView& view, bool color) {
    QChart* chart = new QChart;   // Создаем диаграмму
    chart->setTitle("Pie chart"); // Задаем название

    QPieSeries* series = new QPieSeries(chart);  // Создаем серию данных для диаграммы

    size_t count = 0; // Счетчик наборов данных
    for (auto elem : data)  // Идем по контейнеру данных
    {
        series->append(elem.second, elem.first);   // Добавляем элемент в серию данных
        if (!color) {  // Если нужен не цветной график
            // Создаем черно-белый градиент
            QLinearGradient gradient(0, 0, view.size().width(), view.size().height());
            gradient.setColorAt(0, Qt::black);
            gradient.setColorAt(1, Qt::white);

            QBrush brush(gradient);  // Добавляем кисть от градиента
            series->slices()[count]->setBrush(brush);
        }
        ++count;
    }

    chart->addSeries(series);    // Добавляем серию на диаграмму
    chart->createDefaultAxes();  // Создаем оси
    view.setChart(chart);        // Помещаем диаграмму в представление
}

// Отрисовка вертикальной диаграммы
void barChartDrawer::drawChart(const DataList& data, QChartView& view, bool color) {
    QChart* chart = new QChart;    // Создаем диаграмму
    chart->setTitle("Bar chart");  // Задаем название

    QBarSeries* series = new QBarSeries(chart); // Создаем серию данных для диаграммы

    for (auto elem : data) // Идем по контейнеру данных
    {
        QBarSet* set = new QBarSet(elem.second); // Задаем один набор данных
        *set << elem.first;                           // Помещаем значение элемента в набор
        if (!color) { // Если нужен не цветной график
            // Создаем черно-белый градиент
            QLinearGradient gradient(0, 0, view.size().width(), view.size().height());
            gradient.setColorAt(0, Qt::black);
            gradient.setColorAt(1, Qt::white);

            set->setBrush(QBrush(gradient)); // Добавляем кисть от градиента
        }
        series->append(set);  // Добавляем набор в серию данных
    }

    chart->addSeries(series);    // Добавляем серию на диаграмму
    chart->createDefaultAxes();  // Создаем оси
    view.setChart(chart);        // Помещаем диаграмму в представление
}