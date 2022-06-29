#include "dataReader.h"


void SqlLiteReader::readData(const QString& path, DataList& data) {
    static QSqlDatabase data_base = QSqlDatabase::addDatabase("QSQLITE");
    data_base.setDatabaseName(path);
    if (data_base.open()) {
        data.clear();
        QSqlQuery query("SELECT TIME, VALUE FROM " + data_base.tables().takeFirst());
        for (size_t count = 0; query.next() && count < 10; ++count) {
            QPointF point(count, query.value(1).toDouble());
            data.push_back(Data(point, query.value(0).toString()));
        }
    } else {
        QMessageBox messageBox;
        messageBox.setText("нельзя открыть файл");
        messageBox.exec();
    }
}


void JsonReader::readData(const QString& path, DataList& data) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox messageBox;
        messageBox.setText("Нельзя открыть файл");
        messageBox.exec();
    }

    QString string = file.readAll();
    QJsonDocument json = QJsonDocument::fromJson(string.toUtf8());

    if (!json.isArray()) {
        QMessageBox messageBox;
        messageBox.setText("Введите Json как Массив");
        messageBox.exec();
    }
    data.clear();
    size_t count = 0;
    for (auto value : json.array()) {
        if (value.isObject() && count < 10) {
            QPointF point(count, value.toObject()["Value"].toDouble());
            data.push_back(Data(point, value.toObject()["Time"].toString()));
            ++count;
        }
    }
}