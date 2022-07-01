#include "mainwindow.h"


MainWindow::MainWindow(QMainWindow* parent) : QMainWindow(parent) {
    setGeometry(100, 100, 1600, 800);
    setWindowTitle("Диаграмма");

    chart = new Chart;

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setRootPath(homePath);

    tableW = new tableWidget(fileModel);
    chartW = new chartWidget(&chart->getView());

    QSplitter* splitter = new QSplitter;
    splitter->addWidget(tableW);
    splitter->addWidget(chartW);

    setCentralWidget(splitter);

    selectionModel = tableW->tableView->selectionModel();

    connections();
}


MainWindow::tableWidget::tableWidget(QFileSystemModel* fileModel, QWidget* parent) : QWidget(parent) {
    tableView = new QTableView;
    tableView->setModel(fileModel);

    openButton = new QPushButton ("Открыть папку");
    QVBoxLayout* table = new QVBoxLayout;
    table->addWidget(new QLabel("Выберите базу данных:"));
    table->addWidget(tableView);
    table->addWidget(openButton);

    setLayout(table);
}


MainWindow::chartWidget::chartWidget(QChartView* view, QWidget* parent) : QWidget(parent) {
    printButton = new QPushButton("Печать графика");
    checkboxColor = new QCheckBox("Черно-белый");

    boxType = new QComboBox(); // Выбор типа графика
    boxType->insertItem(0, QString("BarChart"));
    boxType->insertItem(1, QString("PieChart"));

    QHBoxLayout* horizontal = new QHBoxLayout;
    horizontal->addWidget(new QLabel("Выберите тип диаграммы:"));
    horizontal->addWidget(boxType);
    horizontal->addWidget(checkboxColor);
    horizontal->addWidget(printButton);

    QVBoxLayout* vertical = new QVBoxLayout;
    vertical->addLayout(horizontal);
    vertical->addWidget(view);

    setLayout(vertical);
}


void MainWindow::connections() {
    connect(tableW->openButton, SIGNAL(clicked()),this,SLOT(directoryChoose()));
    connect(chartW->boxType,SIGNAL(currentTextChanged(const QString&)),this,SLOT(comboboxChange()));
    connect(chartW->checkboxColor, SIGNAL(toggled(bool)), this, SLOT(colorChange()));
    connect(chartW->printButton,SIGNAL(clicked()), this, SLOT(toPDF()));
    connect(selectionModel,SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,SLOT(selectionChange(const QItemSelection &, const QItemSelection &)));

    IOCContainer::instance().RegisterInstance<IchartDrawer, barChartDrawer>();
}


void MainWindow::directoryChoose() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    if (dialog.exec()) {
        homePath = dialog.selectedFiles().first();
    }
    tableW->tableView->setRootIndex(fileModel->setRootPath(homePath));
}


void MainWindow::comboboxChange() {

    if (chartW->boxType->currentText() == "BarChart") {
        IOCContainer::instance().RegisterInstance<IchartDrawer, barChartDrawer>();
        chart->updateChart();
    } else {
        IOCContainer::instance().RegisterInstance<IchartDrawer, pieChartDrawer>();
        chart->updateChart();
    }
}


void MainWindow::colorChange() {
    chart->changeColor();
    chart->updateChart();
}


void MainWindow::toPDF() {
    if (checkPrint) {
        QFileDialog dialog(this);
        dialog.setViewMode(QFileDialog::Detail);
        QString path;
        if (dialog.exec()) {
            path = dialog.selectedFiles().first();
        }
        QPdfWriter writer(path + ".pdf");
        writer.setCreator("Creator");
        writer.setPageSize(QPagedPaintDevice::A4);
        QPainter painter(&writer);
        chart->getView().render(&painter);
        painter.end();
    } else {
        QMessageBox messageBox;
        messageBox.setText("Отсутствует диаграмма чтобы напечатать");
        messageBox.exec();
    }
}


void MainWindow::selectionChange(const QItemSelection& selected, const QItemSelection& deselected) {
    Q_UNUSED(deselected);

    QString path(fileModel->filePath(selected.indexes().constFirst()));


    if (path.endsWith(".sqlite")) {
        checkPrint = true;
        IOCContainer::instance().RegisterInstance<IdataReader, SqlLiteReader>();
        chart->updateData(path);
        chart->updateChart();
        return;
    }
    if (path.endsWith(".json")) {
        checkPrint = true;
        IOCContainer::instance().RegisterInstance<IdataReader, JsonReader>();
        chart->updateData(path);
        chart->updateChart();
        return;
    }

    checkPrint = false;
    QMessageBox messageBox;
    messageBox.setText("Неверный формат");
    messageBox.exec();
}


//tableWidget::tableWidget(QWidget *parent) {
//    fileModel = new QFileSystemModel(this);
//    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
//    fileModel->setRootPath(homePath);
//
//    tableView = new QTableView;
//    tableView->setModel(fileModel);
//
//    openButton = new QPushButton ("Открыть папку");
//
//    selectionModel = tableView->selectionModel();
//
//    connect(openButton, SIGNAL(clicked()),this,SLOT(directoryChoose()));
//    connect(selectionModel,SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,SLOT(selectionChange(const QItemSelection &, const QItemSelection &)));
//
//    QVBoxLayout* tableLayout = new QVBoxLayout;
//    tableLayout->addWidget(new QLabel("Выберите базу данных:"));
//    tableLayout->addWidget(tableView);
//    tableLayout->addWidget(openButton);
//}
//
//
//void tableWidget::directoryChoose() {
//    QFileDialog dialog(this);
//    dialog.setFileMode(QFileDialog::Directory);
//    if (dialog.exec()) {
//        homePath = dialog.selectedFiles().first();
//    }
//    tableView->setRootIndex(fileModel->setRootPath(homePath));
//}
//
//void tableWidget::selectionChange(const QItemSelection& selected, const QItemSelection& deselected) {
//    Q_UNUSED(deselected);
//
//    QString path(fileModel->filePath(selected.indexes().constFirst()));
//
////    if (path.endsWith(".sqlite")) {
////        checkPrint = true;
////        IOCContainer::instance().RegisterInstance<IdataReader, SqlLiteReader>();
////        chart->updateData(path);
////        chart->updateChart();
////        return;
////    }
////    if (path.endsWith(".json")) {
////        checkPrint = true;
////        IOCContainer::instance().RegisterInstance<IdataReader, JsonReader>();
////        chart->updateData(path);
////        chart->updateChart();
////        return;
////    }
//
////    checkPrint = false;
////    QMessageBox messageBox;
////    messageBox.setText("Неверный формат");
////    messageBox.exec();
//}