#include "functionalwidget.h"

FunctionalWidget::FunctionalWidget(QWidget *parent) : QWidget(parent)
{

}

    void FunctionalWidget::init(ImageProcessingFile *img)
    {
        image=img;
    }
