#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "Chart.h"


class MainWindow : public QWidget {
Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

private slots:
    void directoryChoose();
    void comboboxChange();
    void colorChange();
    void toPDF();
    void selectionChange(const QItemSelection& selected, const QItemSelection& deselected);

private:
    bool checkPrint = false;

    QString homePath = QDir::homePath();
    Chart* chart = nullptr;

    QFileSystemModel* fileModel = nullptr;
    QTableView* tableView = nullptr;
    QPushButton* openButton = nullptr;
    QPushButton* printButton = nullptr;
    QCheckBox* checkboxColor = nullptr;
    QComboBox* boxType = nullptr;
    QItemSelectionModel* selectionModel = nullptr;

    void connections();
};


#endif // MAINWINDOW_H