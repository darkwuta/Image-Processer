#ifndef FUNCTIONALWIDGET_H
#define FUNCTIONALWIDGET_H

#include <QWidget>
#include <imageprocessing.h>
#include <curvechartview.h>

class FunctionalWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FunctionalWidget(QWidget *parent = nullptr);
    void init(ImageProcessingFile *image);
    ImageProcessingFile *image;

    CurveChartView *curveChartView;
signals:

private:


};

#endif // FUNCTIONALWIDGET_H
