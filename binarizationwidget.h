#ifndef BINARIZATIONWIDGET_H
#define BINARIZATIONWIDGET_H

#include <QWidget>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include"curvechartview.h"
#include"imageprocessing.h"

namespace Ui {
class BinarizationWidget;
}

class BinarizationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BinarizationWidget(QWidget *parent = nullptr);
    ~BinarizationWidget();
    void init(ImageProcessingFile *image);



private slots:
    void on_binThreshold_clicked();

    void on_Otsu_clicked();



    void on_AdaptiveThreshold_clicked();


    void on_Threshold_editingFinished();

signals:
    void imageChange();

private slots:
    void ThresholdChange();

    void on_ShowOrigin_clicked();

    void on_Apply_clicked();

    void on_halftone_clicked();

private:
    Ui::BinarizationWidget *ui;
//    cv::Mat *image;
//    cv::Mat originImage;
    ImageProcessingFile *image;

    CurveChartView *curveChartView;

    int th;
};

#endif // BINARIZATIONWIDGET_H
