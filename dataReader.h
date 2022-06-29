#ifndef LAB3_DATAREADER_H
#define LAB3_DATAREADER_H


#include <QtSql>
#include <QMessageBox>


using Data = QPair<QPointF, QString>;
using DataList = QList<Data>;


class IdataReader {
public:
    IdataReader() = default;
    virtual ~IdataReader() = default;

    virtual void readData(const QString& path, DataList& data) = 0;
};


class SqlLiteReader : public IdataReader {
public:
    SqlLiteReader() = default;

    virtual void readData(const QString& path, DataList& data) override;
};


class JsonReader : public IdataReader {
public:
    JsonReader() = default;

    virtual void readData(const QString& path, DataList& data) override;
};


#endif //LAB3_DATAREADER_H