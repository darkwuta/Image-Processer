#include "fliterweight.h"
#include "ui_fliterweight.h"


FliterWeight::FliterWeight(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FliterWeight)
{
    ui->setupUi(this);
}

FliterWeight::~FliterWeight()
{
    delete ui;
}

void FliterWeight::init(ImageProcessingFile *img)
{
    image=img;
}


void FliterWeight::on_Inversion_clicked()
{
    image->cvImage=image->originImage.clone();
    int height=image->originImage.rows;
    int width = image->originImage.cols;
    int channels = image->originImage.channels();

    //灰度图像
    if(channels==1)
    {
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                image->cvImage.at<uchar>(i, j)=255-image->originImage.at<uchar>(i, j);
            }
        }
        emit imageChange();
    }
    //彩色图像
    if(channels==3)
    {
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                    image->cvImage.at<cv::Vec3b>(i, j)[0]=255-image->originImage.at<cv::Vec3b>(i, j)[0];
                    image->cvImage.at<cv::Vec3b>(i, j)[1]=255-image->originImage.at<cv::Vec3b>(i, j)[1];
                    image->cvImage.at<cv::Vec3b>(i, j)[2]=255-image->originImage.at<cv::Vec3b>(i, j)[2];

            }
        }
        emit imageChange();
    }
}

void FliterWeight::on_ShowOrigin_clicked()
{
    image->cvImage=image->originImage.clone();
    emit imageChange();
}



void FliterWeight::on_apply_clicked()
{
    image->originImage=image->cvImage.clone();
}

void FliterWeight::on_meanBlur_clicked()
{
    int kernelSize = 3;
    int n = kernelSize*kernelSize;


    //准备模板
    for(int y=0;y<kernelSize;y++)
    {
        for(int x=0;x<kernelSize;x++)
        {
            kernel[y][x]=1;
        }
    }
    for(int y=0;y<kernelSize;y++)
    {
        for(int x=0;x<kernelSize;x++)
        {
            kernel[y][x]/=n;
        }
    }
    SetKernel();
    on_preview_clicked();
}

void FliterWeight::SetKernel()
{
    ui->kernel1->setText(QString::number(kernel[0][0]));
    ui->kernel2->setText(QString::number(kernel[0][1]));
    ui->kernel3->setText(QString::number(kernel[0][2]));
    ui->kernel4->setText(QString::number(kernel[1][0]));
    ui->kernel5->setText(QString::number(kernel[1][1]));
    ui->kernel6->setText(QString::number(kernel[1][2]));
    ui->kernel7->setText(QString::number(kernel[2][0]));
    ui->kernel8->setText(QString::number(kernel[2][1]));
    ui->kernel9->setText(QString::number(kernel[2][2]));
}


void FliterWeight::on_gaussian_clicked()
{
    int kernelSize = 3;
    double sigma =ui->coefficient->text().toDouble();
    int pad = floor(kernelSize/2);
    //准备高斯滤波的模板,这里后期可以存成数组，就不用计算了

    double a = 1./(2.*M_PI*sigma*sigma);
    double b =2.*sigma*sigma;

    double kernelSum=0;
    for(int i = 0;i<kernelSize;i++)
    {
        for(int j =0;j<kernelSize;j++)
        {
            int y = i-pad;
            int x = j-pad;
            kernel[i][j] = a*exp(-(x*x+y*y)/b);
            kernelSum+=kernel[i][j];
        }
    }

    for(int y = 0;y<kernelSize;y++)
    {
        for(int x =0;x<kernelSize;x++)
        {
            kernel[y][x]/=kernelSum;
        }
    }
    SetKernel();


    on_preview_clicked();
    //开始滤波


}

void FliterWeight::on_sharpen_clicked()
{
    int kernelSize = 3;
    image->cvImage=image->originImage.clone();
    int channels = image->originImage.channels();


    //kernel={{0,1,0},{1,-4,1},{0,1,0}};

    kernel[0][0]=-1;
    kernel[0][1]=-1;
    kernel[0][2]=-1;
    kernel[1][0]=-1;
    kernel[1][1]=9;
    kernel[1][2]=-1;
    kernel[2][0]=-1;
    kernel[2][1]=-1;
    kernel[2][2]=-1;
    SetKernel();
    if(channels==3)
    {
        int kernelSize = 3;
        int height=image->originImage.rows;
        int width = image->originImage.cols;
        int pad = floor(kernelSize/2);
        cv::Mat temp =image->originImage.clone();
        cv::cvtColor(image->originImage,image->originImage,cv::COLOR_BGR2HSV);//转为HSV
        cv::cvtColor(image->cvImage,image->cvImage,cv::COLOR_BGR2HSV);//转为HSV
        double v =0;
        for(int y =0;y<height;y++)
        {
            for(int x = 0;x<width;x++)
            {
                for(int dy = -pad;dy < pad +1;dy++)
                {
                    for(int dx = -pad; dx < pad+1;dx++)
                    {
                        if(((y+dy)>=0)&&((x+dx)>=0)&&((y+dy<height)&&((x+dx)<width)))
                        {
                            v+=(double)image->originImage.at<cv::Vec3b>(y+dy,x+dx)[3]*kernel[dy+pad][dx+pad];
                        }
                    }
                    v = fmax(v, 0);
                    v = fmin(v, 255);
                    image->cvImage.at<cv::Vec3b>(y,x)[3]=(uchar)v;
                }

            }
        }
        cv::cvtColor(image->cvImage,image->cvImage,cv::COLOR_HSV2BGR);//转回RGB
        image->originImage=temp.clone();

    }
    else if(channels==1)
        on_preview_clicked();


    emit imageChange();


}


void FliterWeight::on_preview_clicked()
{
    int kernelSize = 3;
    image->cvImage=image->originImage.clone();
    int height=image->originImage.rows;
    int width = image->originImage.cols;
    int channels = image->originImage.channels();
    int pad = floor(kernelSize/2);
    //ReadKernel();
    if(channels==3)
    {
        double v =0;
        for(int y =0;y<height;y++)
        {
            for(int x = 0;x<width;x++)
            {
                for(int c=0;c<channels;c++)
                {
                    v=0;
                    for(int dy = -pad;dy < pad +1;dy++)
                    {
                        for(int dx = -pad; dx < pad+1;dx++)
                        {
                            if(((y+dy)>=0)&&((x+dx)>=0)&&((y+dy<height)&&((x+dx)<width)))
                            {
                                v+=(double)image->originImage.at<cv::Vec3b>(y+dy,x+dx)[c]*kernel[dy+pad][dx+pad];
                            }
                        }
                    }
                    v = fmax(v, 0);
                    v = fmin(v, 255);
                    image->cvImage.at<cv::Vec3b>(y,x)[c]=v;
                }

            }
        }
    }
    else if(channels==1)
    {
        double v =0;
        for(int y =0;y<height;y++)
        {
            for(int x = 0;x<width;x++)
            {
                v=0;
                for(int dy = -pad;dy < pad +1;dy++)
                {
                    for(int dx = -pad; dx < pad+1;dx++)
                    {
                        if(((y+dy)>=0)&&((x+dx)>=0)&&((y+dy<height)&&((x+dx)<width)))
                        {
                            v+=(double)image->originImage.at<uchar>(y+dy,x+dx)*kernel[dy+pad][dx+pad];
                        }
                    }
                }
                v = fmax(v, 0);
                v = fmin(v, 255);
                image->cvImage.at<uchar>(y,x)=(uchar)v;

            }
        }
        //cv::imshow("a",image->cvImage);
    }
    emit imageChange();
}

void FliterWeight::on_emboss_clicked()
{
    int kernelSize = 3;
    image->cvImage=image->originImage.clone();


    //double kernel[kernel_size][kernel_size] = {{-2, -1, 0}, {-1, 1, 1}, {0, 1, 2}};

    kernel[0][0]=-2;
    kernel[0][1]=-1;
    kernel[0][2]=0;
    kernel[1][0]=-1;
    kernel[1][1]=1;
    kernel[1][2]=1;
    kernel[2][0]=0;
    kernel[2][1]=1;
    kernel[2][2]=2;
    SetKernel();
    on_preview_clicked();
}

void FliterWeight::ReadKernel()
{
    kernel[0][0]=ui->kernel1->text().toDouble();
    kernel[0][1]=ui->kernel2->text().toDouble();
    kernel[0][2]=ui->kernel3->text().toDouble();
    kernel[1][0]=ui->kernel4->text().toDouble();
    kernel[1][1]=ui->kernel5->text().toDouble();
    kernel[1][2]=ui->kernel6->text().toDouble();
    kernel[2][0]=ui->kernel7->text().toDouble();
    kernel[2][1]=ui->kernel8->text().toDouble();
    kernel[2][2]=ui->kernel9->text().toDouble();
}

void FliterWeight::on_Sobel_clicked()
{
    if(ui->Sobel->isChecked())//"表示被选中1"
    {
        ui->Prewitt->setChecked(false);
        ui->Canny->setChecked(false);
        if(ui->Vertical->isChecked())
        {
            //double kernel[kernel_size][kernel_size] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
            kernel[0][0]=1;
            kernel[0][1]=2;
            kernel[0][2]=1;
            kernel[1][0]=0;
            kernel[1][1]=0;
            kernel[1][2]=0;
            kernel[2][0]=-1;
            kernel[2][1]=-2;
            kernel[2][2]=-1;
            SetKernel();

        }
        else if(ui->Horizontal->isChecked())
        {
            //double kernel[kernel_size][kernel_size] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
            kernel[0][0]=1;
            kernel[0][1]=0;
            kernel[0][2]=-1;
            kernel[1][0]=2;
            kernel[1][1]=0;
            kernel[1][2]=-2;
            kernel[2][0]=1;
            kernel[2][1]=-0;
            kernel[2][2]=-1;
            SetKernel();
        }
    }
}

void FliterWeight::on_Horizontal_clicked()//水平方向
{
    if(ui->Horizontal->isChecked())//"表示被选中1"
    {
        ui->Vertical->setChecked(false);
        if(ui->Sobel->isChecked())//sobel算子
        {
            //double kernel[kernel_size][kernel_size] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
            kernel[0][0]=1;
            kernel[0][1]=0;
            kernel[0][2]=-1;
            kernel[1][0]=2;
            kernel[1][1]=0;
            kernel[1][2]=-2;
            kernel[2][0]=1;
            kernel[2][1]=-0;
            kernel[2][2]=-1;
            SetKernel();

        }
        else if(ui->Prewitt->isChecked())//prewitt算子
        {
            //double kernel[kernel_size][kernel_size] = {{-1, 0, -1}, {-1, 0, 1}, {-1, 0, 1}};
            kernel[0][0]=-1;
            kernel[0][1]=0;
            kernel[0][2]=-1;
            kernel[1][0]=-1;
            kernel[1][1]=0;
            kernel[1][2]=1;
            kernel[2][0]=-1;
            kernel[2][1]=-0;
            kernel[2][2]=1;
            SetKernel();
        }
    }
}

void FliterWeight::on_Prewitt_clicked()
{
    if(ui->Prewitt->isChecked())//"表示被选中1"
    {
        ui->Sobel->setChecked(false);
        ui->Canny->setChecked(false);
        if(ui->Vertical->isChecked())
        {
            //double kernel[kernel_size][kernel_size] = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};
            kernel[0][0]=-1;
            kernel[0][1]=-1;
            kernel[0][2]=-1;
            kernel[1][0]=0;
            kernel[1][1]=0;
            kernel[1][2]=0;
            kernel[2][0]=1;
            kernel[2][1]=1;
            kernel[2][2]=1;
            SetKernel();

        }
        else if(ui->Horizontal->isChecked())
        {
            //double kernel[kernel_size][kernel_size] = {{-1, 0, -1}, {-1, 0, 1}, {-1, 0, 1}};
            kernel[0][0]=-1;
            kernel[0][1]=0;
            kernel[0][2]=-1;
            kernel[1][0]=-1;
            kernel[1][1]=0;
            kernel[1][2]=1;
            kernel[2][0]=-1;
            kernel[2][1]=-0;
            kernel[2][2]=1;
            SetKernel();
        }
    }
}

void FliterWeight::on_Vertical_clicked()
{
    if(ui->Vertical->isChecked())//"表示被选中1"
    {
        ui->Horizontal->setChecked(false);
        if(ui->Sobel->isChecked())//sobel算子
        {
            //double kernel[kernel_size][kernel_size] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
            kernel[0][0]=1;
            kernel[0][1]=2;
            kernel[0][2]=1;
            kernel[1][0]=0;
            kernel[1][1]=0;
            kernel[1][2]=0;
            kernel[2][0]=-1;
            kernel[2][1]=-2;
            kernel[2][2]=-1;
            SetKernel();

        }
        else if(ui->Prewitt->isChecked())//prewitt算子
        {
            //double kernel[kernel_size][kernel_size] = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};
            kernel[0][0]=-1;
            kernel[0][1]=-1;
            kernel[0][2]=-1;
            kernel[1][0]=0;
            kernel[1][1]=0;
            kernel[1][2]=0;
            kernel[2][0]=1;
            kernel[2][1]=1;
            kernel[2][2]=1;
            SetKernel();
        }
    }
}

void FliterWeight::on_saveOperator_clicked()
{
    QString fileName =QFileDialog::getSaveFileName(this, tr("保存算子"), "", tr("Allfile(*.*);"));

    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        QString qs[9];
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                qs[i*3+j]=QString::number(kernel[i][j])+"\n";
            }
        }
        for(int i=0;i<9;i++)
        {

            QByteArray qb=qs[i].toUtf8();
            file.write(qb,qb.length());
        }
        file.close();
    }
}

void FliterWeight::on_readOperator_clicked()
{
    QString fileName =QFileDialog::getOpenFileName(this, tr("加载算子"), "", tr("Allfile(*.*);"));
    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QString qs[9];
        for(int i=0;i<9;i++)
        {
            QByteArray qb=file.readLine();
            qs[i]=QString::fromStdString(qb.toStdString());
        }

        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                kernel[i][j]=qs[i*3+j].toDouble();
            }
        }
        SetKernel();
        file.close();
    }
}

void FliterWeight::on_addNoise_clicked()
{
    image->cvImage=image->originImage.clone();
    int height=image->originImage.rows;
    int width = image->originImage.cols;
    int channels = image->originImage.channels();
    int n = ui->common->text().toInt();
    //灰度图像
    if(channels==1)
    {
        for(int i = 0;i<n;i++)
        {
            int w = rand()%width;
            int h = rand()%height;
             image->cvImage.at<uchar>(h, w)=255;
        }
        emit imageChange();
    }
}

void FliterWeight::on_lowPathFliter_clicked()
{
    int kernelSize = 3;
    image->cvImage=image->originImage.clone();
    int height=image->originImage.rows;
    int width = image->originImage.cols;
    int channels = image->originImage.channels();

    double passR=ui->coefficient->text().toDouble();

    if(channels==1)
    {
//        // filtering
//          int r = height / 2;
//          int filter_d = (int)((double)r * passR);
//          for ( int j = 0; j < height / 2; j++){
//            for ( int i = 0; i < width / 2; i++){
//              if (sqrt(i * i + j * j) >= filter_d){
//                image->fourierStr->coef[j][i] = 0;
//                image->fourierStr->coef[j][width - i] = 0;
//                image->fourierStr->coef[height - i][i] = 0;
//                image->fourierStr->coef[height - i][width - i] = 0;
//              }
//            }
//          }

//          FourierStr tmp_s(height,width);
//          // region change
//          for ( int j = 0; j < height / 2; j++){
//            for ( int i = 0; i < width / 2; i++){
//              // left top > right bottom
//              tmp_s.coef[height / 2 + j][width / 2 + i] = image->fourierStr->coef[j][i];
//              // right top > left bottom
//              tmp_s.coef[height / 2 + j][i] = image->fourierStr->coef[j][width / 2 + i];
//              // left bottom > right top
//              tmp_s.coef[j][width / 2 + i] = image->fourierStr->coef[height / 2 + j][i];
//              // right bottom > left top
//              tmp_s.coef[j][i] = image->fourierStr->coef[height / 2 + j][width / 2 + i];
//            }
//          }

//          // filtering
//          int filterD = (int)((double)r / 2);
//          for ( int j = 0; j < height / 2; j++){
//            for ( int i = 0; i < width / 2; i++){
//              if (sqrt(i * i + j + j) >= filter_d){
//                tmp_s.coef[height / 2 - j][width / 2 + i] = 0;
//                tmp_s.coef[height / 2 - j][width / 2 - i] = 0;
//                tmp_s.coef[height / 2 + i][width / 2 + i] = 0;
//                tmp_s.coef[height / 2 + i][width / 2 - i] = 0;
//              }
//            }
//          }

//          // return region
//          for ( int j = 0; j < height / 2; j++){
//            for ( int i = 0; i < width / 2; i++){
//              // left top > right bottom
//              image->fourierStr->coef[height / 2 + j][width / 2 + i] = tmp_s.coef[j][i];
//              // right top > left bottom
//              image->fourierStr->coef[height / 2 + j][i] = tmp_s.coef[j][width / 2 + i];
//              // left bottom > right top
//              image->fourierStr->coef[j][width / 2 + i] = tmp_s.coef[height / 2 + j][i];
//              // right bottom > left top
//              image->fourierStr->coef[j][i] = tmp_s.coef[height / 2 + j][width / 2 + i];
//            }
//          }

    }
//    image->idft();
    emit imageChange();
}

void FliterWeight::on_medianFliter_clicked()
{
    int kernelSize = 3;
    image->cvImage=image->originImage.clone();
    int height=image->originImage.rows;
    int width = image->originImage.cols;
    int channels = image->originImage.channels();
    int pad = floor(kernelSize/2);

    int max=0;
    // filtering
    double v = 0;
    int vs[kernelSize * kernelSize];
    int count = 0;
    if(channels==1)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                v = 0;
                count = 0;
                max = 0;
                    for (int i = 0; i < kernelSize * kernelSize; i++)
                        vs[i] = 999;
                // get neighbor pixels
                for (int dy = -pad; dy < pad + 1; dy++)
                {
                    for (int dx = -pad; dx < pad + 1; dx++)
                        if (((y+dy)>=0)&&((x+dx)>=0)&&((y+dy<height)&&((x+dx)<width)))
                        {
                            vs[count++] = (int)image->originImage.at<uchar>(y + dy, x + dx);
                            if((int)image->originImage.at<uchar>(y + dy, x + dx)>max)
                                max=(int)image->originImage.at<uchar>(y + dy, x + dx);
                        }
                }

                // get and assign median
                std::sort(vs, vs + (kernelSize * kernelSize));
               // image->cvImage.at<uchar>(y, x) =max;
                image->cvImage.at<uchar>(y, x) = (uchar)vs[int(floor(count / 2)) + 1];
            }
          }
         emit imageChange();
    }

}

void FliterWeight::on_Canny_clicked()
{

}

void FliterWeight::on_autoMedianFliter_clicked()
{
    int kernelSize = 3;
    int maxSize = 7;

    image->cvImage=image->originImage.clone();
    int height=image->originImage.rows;
    int width = image->originImage.cols;
    int channels = image->originImage.channels();


    int x=0;
    int y=0;
    bool isChanged=false;
    // filtering
    if(channels==1)
    {
        while(kernelSize<=maxSize)
        {
            //kernelSize=3;
            while(y<height)
            {
                if(x==width)
                    x=0;
                while(x<width)
                {
                    int vs[kernelSize * kernelSize];
                    int pad = floor(kernelSize/2);
                    double v = 0;
                    int count = 0;
                        for (int i = 0; i < kernelSize * kernelSize; i++)
                            vs[i] = 255;
                    // get neighbor pixels
                    for (int dy = -pad; dy < pad + 1; dy++)
                    {
                        for (int dx = -pad; dx < pad + 1; dx++)
                            if (((y+dy)>=0)&&((x+dx)>=0)&&((y+dy<height)&&((x+dx)<width)))
                            {
                                vs[count++] = (int)image->originImage.at<uchar>(y + dy, x + dx);
                            }
                        else
                                vs[count++]=0;
                    }

                    // get and assign median
                    std::sort(vs, vs + (kernelSize * kernelSize));
                   // image->cvImage.at<uchar>(y, x) =max;
                    //step A
                    int min = vs[0];
                    int max = vs[kernelSize * kernelSize-1];
                    int med = vs[kernelSize*kernelSize/2];
                    int zxy = image->originImage.at<uchar>(y,x);


                    if(med>min&&med<max)
                    {
                        //step B
                        if(zxy>min&&zxy<max)
                             image->cvImage.at<uchar>(y, x) = image->originImage.at<uchar>(y, x);
                        else
                        {
                            int a= image->cvImage.at<uchar>(y, x) ;
                             image->cvImage.at<uchar>(y, x) = med;
                        }
                    }
                    else
                    {
                        isChanged = true;
                        kernelSize+=2;
                        if(kernelSize>maxSize)
                        {
                            image->cvImage.at<uchar>(y, x) =med;
                            kernelSize=3;
                            isChanged = false;
                        }
                    }
                    if(isChanged)
                        break;
                    kernelSize=3;
                    x++;
                }
                if(isChanged)
                    break;
                kernelSize=3;
                y++;
            }
            isChanged=false;
            if(x==width&&y==height)
                break;
        }
        }
         emit imageChange();
}
