#ifndef TESTIMPORT_H
#define TESTIMPORT_H

#include <QObject>

class TestImport : public QObject
{
    Q_OBJECT

public:
    TestImport();
    ~TestImport();

private slots:
    void csvImport();
    void csvImport_data();

    void csvQuotedFields();
    void csvQuotedFields_data();

    void csvQuoteField();
    void csvQuoteField_data();
    void csvExportImportRoundTrip();
    void csvExportImportRoundTrip_data();
};

#endif
