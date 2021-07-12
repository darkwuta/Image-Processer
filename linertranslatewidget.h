#ifndef LINERTRANSLATEWIDGET_H
#define LINERTRANSLATEWIDGET_H

#include <QWidget>
#include<functionalwidget.h>

namespace Ui {
class LinerTranslateWidget;
}

class LinerTranslateWidget : public FunctionalWidget
{
    Q_OBJECT

public:
    explicit LinerTranslateWidget(FunctionalWidget *parent = nullptr);
    ~LinerTranslateWidget();

    int func[256];
    double k;
    double b;
private slots:

    void FuncChange();

    void on_linerTranslate_clicked();

    void on_slope_editingFinished();

signals:
    void imageChange();

private:
    Ui::LinerTranslateWidget *ui;
};

#endif // LINERTRANSLATEWIDGET_H
