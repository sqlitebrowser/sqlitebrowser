#ifndef CONDFORMATMANAGER_H
#define CONDFORMATMANAGER_H

#include <QDialog>
#include <vector>

#include "Palette.h"

namespace Ui {
    class CondFormatManager;
}

class CondFormat;
class QTreeWidgetItem;
class QAbstractButton;

class CondFormatManager : public QDialog
{
    Q_OBJECT

public:
    explicit CondFormatManager(const std::vector<CondFormat>& condFormats, const QString& encoding, QWidget *parent = nullptr);
    ~CondFormatManager() override;

    std::vector<CondFormat> getCondFormats() const;
private:
    enum Columns {
        ColumnForeground = 0,
        ColumnBackground,
        ColumnFont,
        ColumnSize,
        ColumnBold,
        ColumnItalic,
        ColumnUnderline,
        ColumnAlignment,
        ColumnFilter
    };
    Ui::CondFormatManager *ui;
    std::vector<CondFormat> m_condFormats;
    Palette m_condFormatPalette;
    QString m_encoding;

private slots:
    void addNewItem();
    void addItem(const CondFormat& aCondFormat);
    void removeItem();
    void moveItem(int offset);
    void upItem();
    void downItem();
    void buttonBoxClicked(QAbstractButton* button);
    
public slots:
    void itemClicked(QTreeWidgetItem* item, int column);
};

#endif // CONDFORMATMANAGER_H
