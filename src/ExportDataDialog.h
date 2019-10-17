#ifndef ExportDataDialog_H
#define ExportDataDialog_H

#include <QDialog>

#include "sql/ObjectIdentifier.h"

class DBBrowserDB;

namespace Ui {
class ExportDataDialog;
}

class ExportDataDialog : public QDialog
{
    Q_OBJECT

public:
    enum ExportFormats
    {
        ExportFormatCsv,
        ExportFormatJson,
    };

    explicit ExportDataDialog(DBBrowserDB& db, ExportFormats format, QWidget* parent = nullptr,
                              const std::string& query = {}, const sqlb::ObjectIdentifier& selection = sqlb::ObjectIdentifier());
    ~ExportDataDialog() override;

private slots:
    void accept() override;
    void showCustomCharEdits();

private:
    void setQuoteChar(const QChar& c);
    char currentQuoteChar() const;

    void setSeparatorChar(const QChar& c);
    char currentSeparatorChar() const;

    void setNewLineString(const QString& s);
    QString currentNewLineString() const;

    bool exportQuery(const std::string& sQuery, const QString& sFilename);
    bool exportQueryCsv(const std::string& sQuery, const QString& sFilename);
    bool exportQueryJson(const std::string& sQuery, const QString& sFilename);

private:
    Ui::ExportDataDialog* ui;
    DBBrowserDB& pdb;

    ExportFormats m_format;

    std::string m_sQuery;
};

#endif
