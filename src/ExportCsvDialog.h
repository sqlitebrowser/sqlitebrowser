#ifndef EXPORTCSVDIALOG_H
#define EXPORTCSVDIALOG_H

#include <QDialog>

class DBBrowserDB;

namespace Ui {
class ExportCsvDialog;
}

class ExportCsvDialog : public QDialog
{
    Q_OBJECT

public:
    enum ExportFormats
    {
        ExportFormatCsv,
        ExportFormatJson,
    };

    explicit ExportCsvDialog(DBBrowserDB* db, ExportFormats format, QWidget* parent = 0, const QString& query = "", const QString& selection = "");
    ~ExportCsvDialog();

private slots:
    virtual void accept();
    void showCustomCharEdits();

private:
    void setQuoteChar(const QChar& c);
    char currentQuoteChar() const;

    void setSeparatorChar(const QChar& c);
    char currentSeparatorChar() const;

    void setNewLineString(const QString& s);
    QString currentNewLineString() const;

    bool exportQuery(const QString& sQuery, const QString& sFilename);
    bool exportQueryCsv(const QString& sQuery, const QString& sFilename);
    bool exportQueryJson(const QString& sQuery, const QString& sFilename);

private:
    Ui::ExportCsvDialog* ui;
    DBBrowserDB* pdb;

    ExportFormats m_format;

    QString m_sQuery;
};

#endif
