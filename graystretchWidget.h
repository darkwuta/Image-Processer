#ifndef GRAYSTRETCH_H
#define GRAYSTRETCH_H

#include <QWidget>
#include <imageprocessing.h>
#include <curvechartview.h>
namespace Ui {
class GrayStretchWidget;
}

class GrayStretchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GrayStretchWidget(QWidget *parent = nullptr);
    ~GrayStretchWidget();

    int X1x;
    int X1y;
    int X2x;
    int X2y;
    void init(ImageProcessingFile *image);

private slots:
    void X1stretchChange();
    void X2stretchChange();

    void on_grayStretch_clicked();


    void on_apply_clicked();

    void on_X1xAxis_editingFinished();

    void on_X1yAxis_editingFinished();

    void on_X2xAxis_editingFinished();

    void on_X2yAxis_editingFinished();

signals:
    void imageChange();

private:
    Ui::GrayStretchWidget *ui;


    ImageProcessingFile *image;

    CurveChartView *curveChartView;
};

#endif // GRAYSTRETCH_H
