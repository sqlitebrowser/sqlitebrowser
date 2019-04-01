#ifndef CONDFORMATMANAGER_H
#define CONDFORMATMANAGER_H

#include <QDialog>

#include "Palette.h"

namespace Ui {
    class CondFormatManager;
}

class CondFormat;
class QTreeWidgetItem;

class CondFormatManager : public QDialog
{
    Q_OBJECT

public:
    explicit CondFormatManager(const QVector<CondFormat>& condFormats, const QString& encoding, QWidget *parent = nullptr);
    ~CondFormatManager() override;

    QVector<CondFormat> getCondFormats();
private:
    enum Columns {
        ColumnForeground = 0,
        ColumnBackground = 1,
        ColumnFilter = 2
    };
    Ui::CondFormatManager *ui;
    QVector<CondFormat> m_condFormats;
    Palette m_condFormatPalette;
    QString m_encoding;
    
private slots:
    void addNewItem();
    void addItem(const CondFormat& aCondFormat);
    void removeItem();
    void upItem();
    void downItem();
    
public slots:
    void itemDoubleClicked(QTreeWidgetItem* item, int column);
};

#endif // CONDFORMATMANAGER_H
