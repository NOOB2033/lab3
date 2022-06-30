#include "dataReader.h"

// Чтение данных файла формата .sqllite
void SqlLiteReader::readData(const QString& path, DataList& data) {
    static QSqlDatabase data_base = QSqlDatabase::addDatabase("QSQLITE"); // Объект базы данных
    data_base.setDatabaseName(path);  // Передаем путь файла
    if (data_base.open()) { // Если открылась
        data.clear();       // Очищаем контейнер для новых данных
        QSqlQuery query("SELECT TIME, VALUE FROM " + data_base.tables().takeFirst()); // Создаем запрос
        for (size_t count = 0; query.next() && count < 10; ++count) { // Считываем данные в контейнер
            data.push_back(Data(query.value(1).toDouble(), query.value(0).toString()));
        }
    } else { // Если не открылась, выдаем сообщение об ошибке
        QMessageBox messageBox;
        messageBox.setText("Ошибка! Нельзя открыть указанный файл");
        messageBox.exec();
    }
}

// Чтение данных файла формата Json
void JsonReader::readData(const QString& path, DataList& data) {
    QFile file(path); // Создаем файл

    // Пытаемся открыть файл
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox messageBox;
        messageBox.setText("Ошибка! Нельзя открыть указанный файл");
        messageBox.exec();
    }

    QString string = file.readAll();                                   // Считываем файл в строку
    QJsonDocument json = QJsonDocument::fromJson(string.toUtf8()); // Создаем Json файл из строки

    if (!json.isArray()) {  // Если файл не представлен в виде массива, выдаем сообщение об ошибке
        QMessageBox messageBox;
        messageBox.setText("Ошибка! Файл формата Json должен быть представлен как массив");
        messageBox.exec();
    }
    data.clear();  // Очищаем контейнер для новых данных
    size_t count = 0;
    for (auto value : json.array()) { // Считываем данные в контейнер
        if (value.isObject() && count < 10) {
            data.push_back(Data(value.toObject()["Value"].toDouble(), value.toObject()["Time"].toString()));
            ++count;
        }
    }
}