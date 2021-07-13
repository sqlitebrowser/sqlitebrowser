#include "ImageViewer.h"
#include "ui_ImageViewer.h"

#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QScrollBar>

ImageViewer::ImageViewer(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ImageViewer),
    m_scale_factor(1.0)
{
    ui->setupUi(this);
    connect(ui->buttonOriginalSize, &QToolButton::clicked, this, [this]{ scaleImage(100); });

    ui->labelView->addAction(ui->actionPrintImage);
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::resetImage()
{
    ui->labelView->setPixmap(QPixmap(0, 0));
}

void ImageViewer::setImage(const QImage& image)
{
    ui->labelView->setPixmap(QPixmap::fromImage(image));

    // If the image is larger than the viewport scale it to fit the viewport.
    // If the image is smaller than the viewport show it in its original size.
    if(image.size().width() > ui->labelView->size().width() || image.size().height() > ui->labelView->size().height())
        ui->buttonFitToWindow->setChecked(true);
    else
        scaleImage(100);
}

void ImageViewer::openPrintImageDialog()
{
    QPrinter printer;
    QPrintPreviewDialog dialog(&printer);

    connect(&dialog, &QPrintPreviewDialog::paintRequested, &dialog, [&](QPrinter* previewPrinter) {
        QPainter painter(previewPrinter);
        QRect rect = painter.viewport();
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        QPixmap pixmap = *ui->labelView->pixmap();
#else
        QPixmap pixmap = ui->labelView->pixmap(Qt::ReturnByValue);
#endif

        QSize size = pixmap.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(pixmap.rect());
        painter.drawPixmap(0, 0, pixmap);
    });

    dialog.exec();
}

void ImageViewer::scaleToFitWindow(bool enabled)
{
    // Enable/disable the automatic resizing of the label inside the scrollbar
    ui->scrollArea->setWidgetResizable(enabled);

    // When disabling the fit to window scaling, revert back to the original image size
    if(!enabled)
        scaleImage(100);
}

void ImageViewer::scaleImage(int scale)
{
    // Make sure the slider is updated when this is called programmatically
    ui->sliderScale->setValue(scale);

    // Update our scale factor
    qreal factor_change = (scale / 100.0) / m_scale_factor;
    m_scale_factor = scale / 100.0;

    // Resize the image
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QPixmap pixmap = *ui->labelView->pixmap();
#else
    QPixmap pixmap = ui->labelView->pixmap(Qt::ReturnByValue);
#endif
    ui->labelView->resize(m_scale_factor * pixmap.size());

    // Uncheck the fit to window button
    ui->buttonFitToWindow->setChecked(false);

    // Fix scroll bars to zoom into center of viewport instead of the upper left corner
    const auto adjust_scrollbar = [](QScrollBar* scroll_bar, qreal factor) {
        scroll_bar->setValue(static_cast<int>(factor * scroll_bar->value() + ((factor - 1) * scroll_bar->pageStep() / 2)));
    };
    adjust_scrollbar(ui->scrollArea->horizontalScrollBar(), factor_change);
    adjust_scrollbar(ui->scrollArea->verticalScrollBar(), factor_change);
}
