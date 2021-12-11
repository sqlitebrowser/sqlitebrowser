#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QFileDialog>

//
// File Extensions Filters
//  - space separated list of file extensions in QString
//  - used during import/export
//  - passed to QFileDialog to filter files shown based on extension
//
// SQLite DB File Extensions
static const QString FILE_FILTER_SQLDB(QObject::tr("SQLite Database Files (*.db *.sqlite *.sqlite3 *.db3)"));

// SQLite DB Project File Extensions
static const QString FILE_FILTER_SQLPRJ(QObject::tr("DB Browser for SQLite Project Files (*.sqbpro)"));
static const QString FILE_EXT_SQLPRJ_DEFAULT(".sqbpro");

// SQL File Extensions Filter
static const QString FILE_FILTER_SQL(QObject::tr("SQL Files (*.sql)"));
static const QString FILE_EXT_SQL_DEFAULT(".sql");

// All Files Extensions Filter
static const QString FILE_FILTER_ALL(QObject::tr("All Files (*)"));

// Text Files Extensions Filter
static const QString FILE_FILTER_TXT(QObject::tr("Text Files (*.txt)"));
static const QString FILE_EXT_TXT_DEFAULT(".txt");

// Comma,Tab,or Delimiter-Separated Values File Extensions Filter
static const QString FILE_FILTER_CSV(QObject::tr("Comma-Separated Values Files (*.csv)"));
static const QString FILE_EXT_CSV_DEFAULT(".csv");
static const QString FILE_FILTER_TSV(QObject::tr("Tab-Separated Values Files (*.tsv)"));
static const QString FILE_FILTER_DSV(QObject::tr("Delimiter-Separated Values Files (*.dsv)"));
static const QString FILE_FILTER_DAT(QObject::tr("Concordance DAT files (*.dat)"));

// JSON File Extensions Filter
static const QString FILE_FILTER_JSON(QObject::tr("JSON Files (*.json *.js)"));
static const QString FILE_EXT_JSON_DEFAULT(".json");

// XML File Extensions Filter
static const QString FILE_FILTER_XML(QObject::tr("XML Files (*.xml)"));
static const QString FILE_EXT_XML_DEFAULT(".xml");

// Binary File Extensions Filter
static const QString FILE_FILTER_BIN(QObject::tr("Binary Files (*.bin *.dat)"));
static const QString FILE_EXT_BIN_DEFAULT(".bin");

// Scalar Vector Graphics File Extensions Filter
static const QString FILE_FILTER_SVG(QObject::tr("SVG Files (*.svg)"));
static const QString FILE_EXT_SVG_DEFAULT(".svg");

// Hex-Dump File Extension Filter
static const QString FILE_FILTER_HEX(QObject::tr("Hex Dump Files (*.dat *.bin)"));

// Dynamic/Shared Objects File Extension Filter
static const QString FILE_FILTER_DYN(QObject::tr("Extensions (*.so *.dylib *.dll)"));

// Initialization File Extension Filter
static const QString FILE_FILTER_INI(QObject::tr("Initialization File (*.ini)"));

enum FileDialogTypes {
    NoSpecificType,

    CreateProjectFile,
    OpenProjectFile,

    CreateDatabaseFile,
    OpenDatabaseFile,

    CreateSQLFile,
    OpenSQLFile,

    OpenCSVFile,

    CreateDataFile,
    OpenDataFile,

    OpenExtensionFile,
    OpenCertificateFile,

    CreateSettingsFile,
    OpenSettingsFile
};

class FileDialog : public QFileDialog
{
    Q_OBJECT

public:
    static QString getOpenFileName(const FileDialogTypes dialogType, QWidget* parent = nullptr, const QString& caption = QString(),
                                   const QString& filter = QString(), QString* selectedFilter = nullptr,
                                   Options options = Options());
    static QStringList getOpenFileNames(const FileDialogTypes dialogType, QWidget* parent = nullptr, const QString& caption = QString(),
                                        const QString& filter = QString(), QString* selectedFilter = nullptr,
                                        Options options = Options());
    static QString getSaveFileName(const FileDialogTypes dialogType, QWidget* parent = nullptr, const QString& caption = QString(),
                                   const QString& filter = QString(), const QString& defaultFileName = QString(), QString* selectedFilter = nullptr,
                                   Options options = Options());
    static QString getExistingDirectory(const FileDialogTypes dialogType, QWidget* parent = nullptr, const QString& caption = QString(),
                                        Options options = Options());

    static QString getSqlDatabaseFileFilter();

private:
    static QString getFileDialogPath(const FileDialogTypes dialogType);
    static void setFileDialogPath(const FileDialogTypes dialogType, const QString& new_path);
};

#endif
