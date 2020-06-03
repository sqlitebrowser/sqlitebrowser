#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget* parent = nullptr);
    ~ImageViewer();

    void resetImage();
    void setImage(const QImage& image);

public slots:
    void openPrintImageDialog();

private:
    Ui::ImageViewer* ui;
};

#endif
