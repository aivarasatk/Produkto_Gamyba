#include "progressbar.h"
#include "ui_progressbar.h"

ProgressBar::ProgressBar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressBar), itemsProcessed(0)
{
   ui->setupUi(this);
   setModal(true);
   setWindowTitle("Ruošiama");
   ui->progressBar->setValue(0);
   connect(ui->progressBar, SIGNAL(valueChanged(int)),
           ui->progressBar, SLOT(setValue(int)));
}

ProgressBar::~ProgressBar()
{
    delete ui;
}

void ProgressBar::fileProcessed(){
    itemsProcessed+=1;
    emit ui->progressBar->valueChanged(getPercentage());
    if(itemsProcessed == itemCount){
        emit filesProcessed();
    }
}

