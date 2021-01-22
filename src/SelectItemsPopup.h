#ifndef SELECTITEMS_H
#define SELECTITEMS_H

#include <QDialog>
#include <QModelIndex>

#include <string>
#include <vector>

class QAbstractButton;

namespace Ui {
class SelectItemsPopup;
}

class SelectItemsPopup : public QDialog
{
    Q_OBJECT

public:
    explicit SelectItemsPopup(const std::vector<std::string>& available, const std::vector<std::string>& selected = {}, QWidget* parent = nullptr);
    ~SelectItemsPopup() override;

    std::vector<std::string> selectedItems() const;

private slots:
    void buttonBoxClicked(QAbstractButton* button);

    void selectItem(const QModelIndex& idx = QModelIndex());
    void unselectItem(const QModelIndex& idx = QModelIndex());

    void moveItemUp();
    void moveItemDown();

protected:
    void resizeEvent(QResizeEvent* ev) override;

private:
    Ui::SelectItemsPopup* ui;

    void moveCurrentItem(bool down);
};

#endif
