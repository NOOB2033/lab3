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
        messageBox.setText("нель");
        messageBox.exec();
    }
}


void JsonReader::readData(const QString& path, DataList& data) {
}