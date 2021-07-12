#ifndef MORPHOLOGYWIDGET_H
#define MORPHOLOGYWIDGET_H

#include <QWidget>
#include <imageprocessing.h>

namespace Ui {
class MorphologyWidget;
}

class MorphologyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MorphologyWidget(QWidget *parent = nullptr);
    ~MorphologyWidget();

    void init(ImageProcessingFile *cvImage);

    ImageProcessingFile *image;

    void Dilate(int N);

    void Erode(int N);

    void recursion(int x,int y, bool** &is_looked,int width,int height,uchar k);

    void getConstructCheck();
signals:
    void imageChange();

private slots:
    void on_dilate_clicked();

    void on_erode_clicked();

    void on_apply_clicked();

    void on_showOrigin_clicked();

    void on_openingOperation_clicked();

    void on_closingOperation_clicked();

    void on_edgeExtraction_clicked();

    void on_areaCount_clicked();


    void on_frontGround_clicked();

    void on_backGround_clicked();

private:
    Ui::MorphologyWidget *ui;
    bool construct[3][3];
    int objectCounts;

};

#endif // MORPHOLOGYWIDGET_H
