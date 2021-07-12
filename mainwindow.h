#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <imageprocessing.h>
#include <curvechartview.h>
#include <fliterweight.h>
#include <binarizationwidget.h>
#include <graychangewidget.h>
#include <graystretchwidget.h>
#include<morphologywidget.h>
#include<linertranslatewidget.h>

//拖拽事件
#include <QDragEnterEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

//读取文件
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void dragEnterEvent(QDragEnterEvent *event);//拖动图片
    void dropEvent(QDropEvent *event);//显示图片
    void resizeEvent(QResizeEvent *event);//重写界面

    void GetHistogram();

    void rotate();
    void translate();
    void scale();



private slots:
    void on_readImage_clicked();

    void on_writeImage_clicked();

    void on_ImageInfo_clicked();

    void on_Histogram_clicked();

    void on_Inversion_clicked();

    void on_graying_clicked();

    void on_pseudoColor_clicked();

    void on_brightnessChange_sliderMoved(int position);



    void on_equalization_clicked();

    void on_normalization_clicked();

    void on_colorTable_clicked();

    void on_graying_2_clicked();

    void on_binarization_clicked();

//    void on_thresholdGraying_clicked();

//    void on_OtsuGraying_clicked();

//    void on_AdaptiveThreshold_clicked();

    void on_grayStretch_clicked();

    void on_Fliter_clicked();

    void on_grayChange_clicked();

    void on_rotate_sliderMoved(int position);

    void on_rotate_valueChanged(int value);

    void on_xAxis_sliderMoved(int position);

    void on_xAxis_valueChanged(int value);

    void on_yAxis_sliderMoved(int position);

    void on_yAxis_valueChanged(int value);

    void on_Xscale_sliderMoved(int position);

    void on_Xscale_valueChanged(int value);

    void on_Yscale_sliderMoved(int position);

    void on_Yscale_valueChanged(int value);

    void on_apply_clicked();

    void on_showOrigin_clicked();

    void on_morphology_clicked();

    void on_linerTranslate_clicked();

public slots:
    void ImageChange();

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;

    ImageProcessingFile image;


    QChart *histogram;   //灰度直方图
    QChartView *chartview;
    QWidget *Histogram;


    //二值化
    BinarizationWidget *binarizationWidget;

    //灰度化
    GrayChangeWidget *grayChangeWiget;

    //灰度拉伸
    GrayStretchWidget *grayStretchWidget;

    //滤镜面板
    FliterWeight *fliterWeight;

    //形态学面板
    MorphologyWidget *morphologyWidget;

    //线性变换模板
    LinerTranslateWidget *linerTranslateWidget;

};
#endif // MAINWINDOW_H
