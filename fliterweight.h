#ifndef FLITERWEIGHT_H
#define FLITERWEIGHT_H

#include <QWidget>
#include <imageprocessing.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

namespace Ui {
class FliterWeight;
}

class FliterWeight : public QWidget
{
    Q_OBJECT

public:
    explicit FliterWeight(QWidget *parent = nullptr);
    ~FliterWeight();

//    void GaussianFliter(float sigma, int kernelSize);
    void init(ImageProcessingFile *cvImage);

    ImageProcessingFile *image;

private slots:
//    void on_gaussianFliter_clicked();

    void on_Inversion_clicked();

    void on_ShowOrigin_clicked();


    void on_apply_clicked();

    void on_meanBlur_clicked();

    void on_gaussian_clicked();

    void on_sharpen_clicked();

    void on_preview_clicked();

    void on_emboss_clicked();

    void on_Sobel_clicked();

    void on_Horizontal_clicked();

    void on_Prewitt_clicked();

    void on_Vertical_clicked();

    void on_saveOperator_clicked();

    void on_readOperator_clicked();

    void on_addNoise_clicked();

    void on_lowPathFliter_clicked();

    void on_medianFliter_clicked();

    void on_Canny_clicked();

    void on_autoMedianFliter_clicked();

signals:
    void imageChange();

private:
    Ui::FliterWeight *ui;

    void ReadKernel();

    void SetKernel();


    double kernel[3][3];



};

#endif // FLITERWEIGHT_H
