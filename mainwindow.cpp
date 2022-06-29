#include "mainwindow.h"


MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    setGeometry(100, 100, 1600, 800);
    setWindowTitle("Диаграмма");

    chart = new Chart;
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setRootPath(homePath);
    tableView = new QTableView;
    tableView->setModel(fileModel);
    openButton = new QPushButton ("Открыть папку");
    printButton = new QPushButton ("Печать графика");
    checkboxColor = new QCheckBox("Черно-белый");
    boxType = new QComboBox(); // Выбор типа графика
    boxType->insertItem(0, QString("BarChart"));
    boxType->insertItem(1, QString("PieChart"));

    selectionModel = tableView->selectionModel();

    QLabel* checkLabel = new QLabel("Выберите тип диаграммы:");
    QLabel* tableLabel = new QLabel("Выберите базу данных:");

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(checkLabel);
    buttonLayout->addWidget(boxType);
    buttonLayout->addWidget(checkboxColor);
    buttonLayout->addWidget(printButton);

    QVBoxLayout* tableLayout = new QVBoxLayout;
    tableLayout->addWidget(tableLabel);
    tableLayout->addWidget(tableView);
    tableLayout->addWidget(openButton);

    QVBoxLayout* chartLayout = new QVBoxLayout;
    chartLayout->addLayout(buttonLayout);
    chartLayout->addWidget(&chart->getView());

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(tableLayout);
    mainLayout->addLayout(chartLayout);

    connections();
}


void MainWindow::connections() {
    connect(openButton, SIGNAL(clicked()),this,SLOT(directoryChoose()));
    connect(boxType,SIGNAL(currentTextChanged(const QString&)),this,SLOT(comboboxChange()));
    connect(checkboxColor, SIGNAL(toggled(bool)), this, SLOT(colorChange()));
    connect(printButton,SIGNAL(clicked()), this, SLOT(toPDF()));
    connect(selectionModel,SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,SLOT(selectionChange(const QItemSelection &, const QItemSelection &)));

    IOCContainer::instance().RegisterInstance<IchartDrawer, barChartDrawer>();
}


void MainWindow::directoryChoose() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    if (dialog.exec()) {
        homePath = dialog.selectedFiles().first();
    }
    tableView->setRootIndex(fileModel->setRootPath(homePath));
}


void MainWindow::comboboxChange() {

    if (boxType->currentText() == "BarChart") {
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