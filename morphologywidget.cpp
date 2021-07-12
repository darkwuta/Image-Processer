#include "morphologywidget.h"
#include "ui_morphologywidget.h"

MorphologyWidget::MorphologyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MorphologyWidget)
{
    ui->setupUi(this);
}

void MorphologyWidget::init(ImageProcessingFile *img)
{
    image = img;
}

MorphologyWidget::~MorphologyWidget()
{
    delete ui;
}

void MorphologyWidget::on_dilate_clicked()//膨胀
{
    image->cvImage=image->originImage.clone();
    int Erode_time=ui->erodeTime->text().toInt();
    Dilate(Erode_time);
    emit imageChange();
}

void MorphologyWidget::on_erode_clicked()//腐蚀
{
    image->cvImage=image->originImage.clone();
    int Erode_time=ui->erodeTime->text().toInt();
    Erode(Erode_time);
    emit imageChange();
}

void MorphologyWidget::on_openingOperation_clicked()
{
    //先腐蚀，再膨胀
    image->cvImage=image->originImage.clone();
    int Erode_time=ui->erodeTime->text().toInt();
    Dilate(Erode_time);
    Erode(Erode_time);
    emit imageChange();
}

void MorphologyWidget::on_closingOperation_clicked()
{
    //先膨胀，再腐蚀
    image->cvImage=image->originImage.clone();
    int Erode_time=ui->erodeTime->text().toInt();
    Erode(Erode_time);
    Dilate(Erode_time);
    emit imageChange();
}

void MorphologyWidget::on_apply_clicked()
{
    image->originImage=image->cvImage.clone();
}

void MorphologyWidget::on_showOrigin_clicked()
{
    image->cvImage=image->originImage.clone();
    emit imageChange();
}

void MorphologyWidget::Dilate(int Erode_time)
{
    //TODO 只有er'zhi2图像才能处理，要考虑设置鲁棒性
    int height=image->originImage.rows;
    int width = image->originImage.cols;

    int kernelSize = 3;
    int channels = image->originImage.channels();
    int pad = floor(kernelSize/2);

    getConstructCheck();
    if(channels==1)
    {
        cv::Mat tmp=image->originImage.clone();
         // for erode time
         for (int i = 0; i < Erode_time; i++)
         {
            tmp = image->cvImage.clone();
           // each pixel
           for (int y = 0; y < height; y++)
           {
             for (int x = 0; x < width; x++)
             {
                 for (int dy = -pad; dy < pad + 1; dy++)
                 {
                     for (int dx = -pad; dx < pad + 1; dx++)
                     {
                         if (((y+dy)>=0)&&((x+dx)>=0)&&((y+dy<height)&&((x+dx)<width))&&(tmp.at<uchar>(y+dy, x+dx) == 255)&&construct[dx][dy])
                         {
                             image->cvImage.at<uchar>(y, x)=255;
                             break;
                         }
                     }
                     if(image->cvImage.at<uchar>(y, x)==255)
                        break;
                 }


//               // check left pixel
//               if (&&(x > 0) && (tmp.at<uchar>(y, x - 1) == 0)){
//                image->cvImage.at<uchar>(y, x) = 0;
//                 continue;
//               }

//               // check up pixel
//               if ((y > 0) && (tmp.at<uchar>(y - 1, x) == 0)){
//                 image->cvImage.at<uchar>(y, x) = 0;
//                 continue;
//               }

//               // check right pixel
//               if ((x < width - 1) && (tmp.at<uchar>(y, x + 1) == 0)){
//                 image->cvImage.at<uchar>(y, x) = 0;
//                 continue;
//               }

//               // check left pixel
//               if ((y < height - 1) && (tmp.at<uchar>(y + 1, x) == 0)){
//                 image->cvImage.at<uchar>(y, x) = 0;
//                 continue;
//               }
             }
           }
         }
    }
}

void MorphologyWidget::Erode(int Erode_time)
{
    //TODO 只有er'zhi2图像才能处理，要考虑设置鲁棒性
    int height=image->originImage.rows;
    int width = image->originImage.cols;

    int kernelSize = 3;
    int channels = image->originImage.channels();
    int pad = floor(kernelSize/2);

    getConstructCheck();
    if(channels==1)
    {
        cv::Mat tmp=image->originImage.clone();
         // for erode time
         for (int i = 0; i < Erode_time; i++)
         {
            tmp = image->cvImage.clone();
           // each pixel
           for (int y = 0; y < height; y++)
           {
             for (int x = 0; x < width; x++)
             {
                 for (int dy = -pad; dy < pad + 1; dy++)
                 {
                     for (int dx = -pad; dx < pad + 1; dx++)
                     {
                         if (((y+dy)>=0)&&((x+dx)>=0)&&((y+dy<height)&&((x+dx)<width))&&(tmp.at<uchar>(y+dy, x+dx) == 0)&&construct[dx][dy])
                         {
                             image->cvImage.at<uchar>(y, x)=0;
                             break;
                         }
                     }
                     if(image->cvImage.at<uchar>(y, x)==0)
                        break;
                 }


//               // check left pixel
//               if (&&(x > 0) && (tmp.at<uchar>(y, x - 1) == 0)){
//                image->cvImage.at<uchar>(y, x) = 0;
//                 continue;
//               }

//               // check up pixel
//               if ((y > 0) && (tmp.at<uchar>(y - 1, x) == 0)){
//                 image->cvImage.at<uchar>(y, x) = 0;
//                 continue;
//               }

//               // check right pixel
//               if ((x < width - 1) && (tmp.at<uchar>(y, x + 1) == 0)){
//                 image->cvImage.at<uchar>(y, x) = 0;
//                 continue;
//               }

//               // check left pixel
//               if ((y < height - 1) && (tmp.at<uchar>(y + 1, x) == 0)){
//                 image->cvImage.at<uchar>(y, x) = 0;
//                 continue;
//               }
             }
           }
         }
    }

}

void MorphologyWidget::recursion(int x, int y, bool **&is_looked, int width, int height, uchar k)
{
    if(is_looked[y][x]==true)
    {
        return;
    }
    is_looked[y][x]=true;
    for (int dy = -1; dy < 2; dy++)
    {
        for (int dx = -1; dx < 2; dx++)
        {
            if (((y+dy)>=0)&&((x+dx)>=0)&&((y+dy<height)&&((x+dx)<width))&&image->cvImage.at<uchar>(y+dy, x+dx)==k&&!is_looked[y+dy][x+dx])
            {
                recursion(x+dx,y+dy,is_looked,width,height,k);
            }

        }
    }
}




void MorphologyWidget::on_edgeExtraction_clicked()
{
    int height=image->originImage.rows;
    int width = image->originImage.cols;

    cv::Mat tmp=image->originImage.clone();
    for (int y = 0; y < height; y++){
            for (int x = 0; x < width; x++){
                 image->cvImage.at<uchar>(y, x) = image->originImage.at<uchar>(y, x)-image->cvImage.at<uchar>(y, x);
            }
    }
    emit imageChange();
}


 void MorphologyWidget::getConstructCheck()
 {
     construct[0][0]=ui->construct1->isChecked();
     construct[0][1]=ui->construct2->isChecked();
     construct[0][2]=ui->construct3->isChecked();
     construct[1][0]=ui->construct4->isChecked();
     construct[1][1]=ui->construct5->isChecked();
     construct[1][2]=ui->construct6->isChecked();
     construct[2][0]=ui->construct7->isChecked();
     construct[2][1]=ui->construct8->isChecked();
     construct[2][2]=ui->construct9->isChecked();

 }

void MorphologyWidget::on_areaCount_clicked()
{
    //选择前景还是后景，前景255，背景0,默认前景
    uchar k =255;
    if(ui->frontGround->isChecked())
        k=255;
    else
        k=0;
    objectCounts=0;
    getConstructCheck();
    int height=image->originImage.rows;
    int width = image->originImage.cols;
    bool **is_looked=new bool*[height];
    for(int i=0;i<height;i++)
        is_looked[i]=new bool[width];
    for (int y = 0; y < height; y++){
            for (int x = 0; x < width; x++){
                 is_looked[y][x]=false;
            }
    }

    for (int y = 0; y < height; y++){
            for (int x = 0; x < width; x++){
                if(!is_looked[y][x]&&image->cvImage.at<uchar>(y, x) == k)
                {
                    recursion(x,y,is_looked,width,height,k);
                    objectCounts++;
                }

            }
    }
    ui->objectCounts->setNum(objectCounts);

}

void MorphologyWidget::on_frontGround_clicked()
{
    if(ui->frontGround->isChecked())//"表示被选中1"
    {
        ui->backGround->setChecked(false);
    }
}

void MorphologyWidget::on_backGround_clicked()
{
    if(ui->backGround->isChecked())//"表示被选中1"
    {
        ui->frontGround->setChecked(false);
    }
}
