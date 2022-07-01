#include "mainwindow.h"

// Конструктор главного окна
MainWindow::MainWindow(QMainWindow* parent) : QMainWindow(parent) {
    setGeometry(100, 100, 1500, 600); // Задаем размер
    setWindowTitle("Диаграмма");                    // Название

    chart = new Chart;                              // Создаем диаграмму

    fileModel = new QFileSystemModel(this);   // Создаем модель файловой системы
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setRootPath(homePath); // Задаем путь
    // Создаем виджеты
    tableW = new tableWidget(fileModel);
    chartW = new chartWidget(&chart->getView());
    // Создаем сплиттер и добавляем ему виджеты
    QSplitter* splitter = new QSplitter;
    splitter->addWidget(tableW);
    splitter->addWidget(chartW);
    // Делаем сплитер центральным виджетом
    setCentralWidget(splitter);
    // Задаем модель для отслеживания файлов
    selectionModel = tableW->tableView->selectionModel();
    // Выполняем соединения сингалов-слотов
    connections();
}

// Конструктор виджета, отвечающего за таблицу папок
MainWindow::tableWidget::tableWidget(QFileSystemModel* fileModel, QWidget* parent) : QWidget(parent) {
    // Создаем табличное представление
    tableView = new QTableView;
    tableView->setModel(fileModel);
    // Создаем кнопку
    openButton = new QPushButton ("Открыть папку");
    // Создаем вертикальный Layout
    QVBoxLayout* table = new QVBoxLayout;
    table->addWidget(new QLabel("Выберите базу данных:"));
    table->addWidget(tableView);
    table->addWidget(openButton);
    // добавляем layout на виджет
    setLayout(table);
}

// Конструктор виджета, отвечающего за диаграмму
MainWindow::chartWidget::chartWidget(QChartView* view, QWidget* parent) : QWidget(parent) {
    // Создаем кнопку
    printButton = new QPushButton("Печать графика");
    // Создаем чекбокс
    checkboxColor = new QCheckBox("Черно-белый");
    // Создаем combobox
    boxType = new QComboBox(); // Выбор типа графика
    boxType->insertItem(0, QString("BarChart"));
    boxType->insertItem(1, QString("PieChart"));
    // Создаем горизонтальный layout
    QHBoxLayout* horizontal = new QHBoxLayout;
    horizontal->addWidget(new QLabel("Выберите тип диаграммы:"));
    horizontal->addWidget(boxType);
    horizontal->addWidget(checkboxColor);
    horizontal->addWidget(printButton);
    // Создаем вертикальный layout
    QVBoxLayout* vertical = new QVBoxLayout;
    vertical->addLayout(horizontal);
    vertical->addWidget(view);
    // добавляем layout на виджет
    setLayout(vertical);
}

// Функция соединения сигнало-слотов
void MainWindow::connections() {
    connect(tableW->openButton, SIGNAL(clicked()),this,SLOT(directoryChoose()));
    connect(chartW->boxType,SIGNAL(currentTextChanged(const QString&)),this,SLOT(comboboxChange()));
    connect(chartW->checkboxColor, SIGNAL(toggled(bool)), this, SLOT(colorChange()));
    connect(chartW->printButton,SIGNAL(clicked()), this, SLOT(toPDF()));
    connect(selectionModel,SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,SLOT(selectionChange(const QItemSelection &, const QItemSelection &)));
    // Добавляем в контейнер по умолчанию вертикальный типа диаграммы
    IOCContainer::instance().RegisterInstance<IchartDrawer, barChartDrawer>();
}

// Слот обработки выбора выбранной папки
void MainWindow::directoryChoose() {
    QFileDialog dialog(this); // Открываем диалог
    dialog.setFileMode(QFileDialog::Directory);
    if (dialog.exec()) {
        homePath = dialog.selectedFiles().first(); // Получаем выбранную директорию
    }
    tableW->tableView->setRootIndex(fileModel->setRootPath(homePath)); // устанавливаем этот путь
}

// Слот обработки выбора типа диаграммы
void MainWindow::comboboxChange() {

    if (chartW->boxType->currentText() == "BarChart") { // Задаем вертикальную диаграмму
        IOCContainer::instance().RegisterInstance<IchartDrawer, barChartDrawer>();
        chart->updateChart();
    } else { // в противном случае - круговую
        IOCContainer::instance().RegisterInstance<IchartDrawer, pieChartDrawer>();
        chart->updateChart();
    }
}

// Слот смены цвета
void MainWindow::colorChange() {
    chart->changeColor(); // Меняем цвет
    chart->updateChart(); // Обновляем диаграмму
}

// Слот печать диаграммы в PDF
void MainWindow::toPDF() {
    if (checkPrint) { // Если можно печатать
        QFileDialog dialog(this); // создаем диалог
        dialog.setViewMode(QFileDialog::Detail);
        QString path;
        if (dialog.exec()) { // Получаем из диалога путь сохранения
            path = dialog.selectedFiles().first();
        } // Добавляем расширение
        QPdfWriter writer(path + ".pdf");
        writer.setCreator("Creator"); // указываем создателя
        writer.setPageSize(QPagedPaintDevice::A4); // размер страницы
        QPainter painter(&writer);
        chart->getView().render(&painter); // Печатаем
        painter.end();
    } else { // В противном случае сообщение об ошибке
        QMessageBox messageBox;
        messageBox.setText("Ошибка! Отсутствует диаграмма чтобы напечатать");
        messageBox.exec();
    }
}

// Слот для выбора файла
void MainWindow::selectionChange(const QItemSelection& selected, const QItemSelection& deselected) {
    Q_UNUSED(deselected);
    // Получаем путь к файалу
    QString path(fileModel->filePath(selected.indexes().constFirst()));

    // если формат .sqllite
    if (path.endsWith(".sqlite")) {
        checkPrint = true; // Разрешаем печатать
        // Добавляем в контейнер
        IOCContainer::instance().RegisterInstance<IdataReader, SqlLiteReader>();
        // Обновляем диаграмму
        chart->updateData(path);
        chart->updateChart();
        return;
    } // если формат .Json
    if (path.endsWith(".json")) {
        checkPrint = true; // Разрешаем печатать
        // Добавляем в контейнер
        IOCContainer::instance().RegisterInstance<IdataReader, JsonReader>();
        // Обновляем диаграмму
        chart->updateData(path);
        chart->updateChart();
        return;
    }
    // В противном случае запрещаем распечатывать и выдаем сообщение об ошибке
    checkPrint = false;
    QMessageBox messageBox;
    messageBox.setText("Ошибка! Неверный формат");
    messageBox.exec();
}