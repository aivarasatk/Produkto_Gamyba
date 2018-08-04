#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QDialog>

namespace Ui {
class ProgressBar;
}

class ProgressBar : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressBar(QWidget *parent = 0);
    ~ProgressBar();

    void fileProcessed();
    int getPercentage(){return ((double)itemsProcessed)/itemCount * 100;}
signals:
    void filesProcessed();//signalas uzdaryt langa

private:
    Ui::ProgressBar *ui;
    const static int itemCount = 2;
    int itemsProcessed;
};

#endif // PROGRESSBAR_H
