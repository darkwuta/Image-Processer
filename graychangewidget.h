#ifndef GRAYCHANGEWIDGET_H
#define GRAYCHANGEWIDGET_H

#include <QWidget>
#include "curvechartview.h"
#include "imageprocessing.h"

namespace Ui {
class GrayChangeWidget;
}

class GrayChangeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GrayChangeWidget(QWidget *parent = nullptr);
    ~GrayChangeWidget();

    int up;
    int low;

    void init(ImageProcessingFile *image);
signals:
    void imageChange();



private slots:
    void on_upperLimit_editingFinished();

    void on_lowerLimit_editingFinished();

    void on_grayChange_clicked();

    void upperLimitChange();
    void lowerLimitChange();


    void on_apply_clicked();

private:
    Ui::GrayChangeWidget *ui;

    ImageProcessingFile *image;

    CurveChartView *curveChartView;


};

#endif // GRAYCHANGEWIDGET_H
