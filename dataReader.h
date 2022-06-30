#ifndef LAB3_DATAREADER_H
#define LAB3_DATAREADER_H


#include <QtSql>
#include <QMessageBox>


using Data = QPair<double, QString>; // Данные - пара<значение, строка(дата-время)>
using DataList = QList<Data>;        // Контейнер с данными

// Интерфейс для считывания данных из файлов
class IdataReader {
public:
    IdataReader() = default;
    virtual ~IdataReader() = default;
    // Метод, считывающий данные в DataList
    virtual void readData(const QString& path, DataList& data) = 0;
};

// Класс для чтения данных из файла формата sqllite
class SqlLiteReader : public IdataReader {
public:
    SqlLiteReader() = default;

    virtual void readData(const QString& path, DataList& data) override;
};

// Класс для чтения данных из файла формата Json
class JsonReader : public IdataReader {
public:
    JsonReader() = default;

    virtual void readData(const QString& path, DataList& data) override;
};


#endif //LAB3_DATAREADER_H