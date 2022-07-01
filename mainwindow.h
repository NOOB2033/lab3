#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "Chart.h"

// Класс главного окна
class MainWindow : public QMainWindow {
Q_OBJECT
public:
    // Конструкторы и деструктор
    MainWindow(QMainWindow* parent = nullptr);
    ~MainWindow() = default;
// Слоты
private slots:
    void directoryChoose(); // Обработка выбора папки
    void comboboxChange();  // Обработка выбора типа диаграммы
    void colorChange();     // Обработка выбора цвета диаграммы
    void toPDF();           // Обработка сохранения диаграммы в PDF
    // Обработка выбора выбранного файла
    void selectionChange(const QItemSelection& selected, const QItemSelection& deselected);

private:
    // Приватный класс-виджет, отвечающий за табличное представление папок
    class tableWidget : public QWidget {
    public:
        // Конструктор, принимающий файловую модель
        tableWidget(QFileSystemModel* fileModel, QWidget* parent = nullptr);

        QTableView* tableView = nullptr;  // Табличной представление
        QPushButton* openButton = nullptr;// Кнопка "Открыть папку"
    };
    // Приватный класс-виджет, отвечающий за диаграмму
    class chartWidget : public QWidget {
    public:
        // Конструктор, принимающий представление диаграммы
        chartWidget(QChartView* view, QWidget* parent = nullptr);

        QPushButton* printButton = nullptr; // Кнопка "Печать графика"
        QCheckBox* checkboxColor = nullptr; // Чекбокс для смены цвета
        QComboBox* boxType = nullptr;       // Список видов диаграмм
    };
    // Поля
    bool checkPrint = false; // Проверка на то, можно ли печатать PDF

    QString homePath = QDir::homePath();           // Текущий путь
    Chart* chart = nullptr;                        // Диаграмма
    QFileSystemModel* fileModel = nullptr;         // Файловая модель
    tableWidget* tableW = nullptr;                 // Объект класса виджета, отвечающего за табличное представление
    chartWidget* chartW = nullptr;                 // Объект класса виджета, отвечающего за диаграмму
    QItemSelectionModel* selectionModel = nullptr; // Модель для отслеживания элементов

    void connections();                            // Функция, соединяющая сигналы-слоты
};


#endif // MAINWINDOW_H