#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "Chart.h"


class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QMainWindow* parent = nullptr);
    ~MainWindow() = default;

private slots:
    void directoryChoose();
    void comboboxChange();
    void colorChange();
    void toPDF();
    void selectionChange(const QItemSelection& selected, const QItemSelection& deselected);

private:

    class tableWidget : public QWidget {
    public:
        tableWidget(QFileSystemModel* fileModel, QWidget* parent = nullptr);

        QTableView* tableView = nullptr;
        QPushButton* openButton = nullptr;
    };

    class chartWidget : public QWidget {
    public:
        chartWidget(QChartView* view, QWidget* parent = nullptr);

        QPushButton* printButton = nullptr;
        QCheckBox* checkboxColor = nullptr;
        QComboBox* boxType = nullptr;
    };

    bool checkPrint = false;

    QString homePath = QDir::homePath();
    Chart* chart = nullptr;
    QFileSystemModel* fileModel = nullptr;
    tableWidget* tableW = nullptr;
    chartWidget* chartW = nullptr;
    QItemSelectionModel* selectionModel = nullptr;

    void connections();
};


//class tableWidget : QWidget {
//Q_OBJECT
//public:
//    tableWidget(QWidget* parent = nullptr);
//
//    QString getPath();
//private slots:
//    void directoryChoose();
//    void selectionChange(const QItemSelection& selected, const QItemSelection& deselected);
//
//    private:
//    QString homePath = QDir::homePath();
//
//    QFileSystemModel* fileModel = nullptr;
//    QTableView* tableView = nullptr;
//    QPushButton* openButton = nullptr;
//    QItemSelectionModel* selectionModel = nullptr;
//};


#endif // MAINWINDOW_H