#include "fileselection.h"
#include "eipfilehandler.h"
#include "ui_fileselection.h"

#include <QMessageBox>
#include <QDebug>
#include <QFileInfo>
#include <QThreadPool>

FileSelection::FileSelection(QWidget *parent, FileData &containers) :
    QDialog(parent),
    ui(new Ui::FileSelection),
    orgContainers(containers)
{
    ui->setupUi(this);
    filesSelected = false;
}

FileSelection::~FileSelection()
{
    delete ui;
}

void FileSelection::prepareDataFiles()
{
    std::vector<int> columnsToRead,parseNumsAt;
    EipFileHandler eipReader;
    eipReader.readHowManyLinesToSkip("superImportantFile.txt",orgContainers);
    eipReader.readEipFile(ingredientFile,orgContainers);
    progressBar->fileProcessed();

    prepareCalculationCardFile(columnsToRead,parseNumsAt);
    prepareDataFile(calculationFile, orgContainers.getCalculationCards(),columnsToRead,parseNumsAt,5);
    progressBar->fileProcessed();
}

void FileSelection::prepareDataFile(QString fileName, std::multiset<std::vector<QString>> &container,
                                 std::vector<int> columnsToRead, std::vector<int> parseNumsAt, int startAt)
{
    ExcelData ed(fileName,container);
    try
    {
        ed.parseExcelFile(columnsToRead,startAt,parseNumsAt,0);
    }catch(std::string excep)
    {
        QMessageBox::critical(this,"Error", QString::fromStdString(excep));
        exit(1);
    }
}

void FileSelection::prepareCalculationCardFile(std::vector<int> &columnsToRead, std::vector<int> &parseNumsAt)
{
    columnsToRead.clear();
    parseNumsAt.clear();
    for(int i = 2; i<11; ++i)
        columnsToRead.push_back(i);

    parseNumsAt.push_back(6);
    parseNumsAt.push_back(9);
}

void FileSelection::on_pushButtonExcel_clicked()
{
    fileSelect.selectFile(this, "*.xlsx");
    calculationFile = fileSelect.getName();
    if(!calculationFile.isEmpty()){
        ui->lineEditExcel->setText( (new QFileInfo(calculationFile))->fileName() );
    }
}

void FileSelection::on_pushButtonEip_clicked()
{
    fileSelect.selectFile(this, "*.eip");
    ingredientFile = fileSelect.getName();
    if(!ingredientFile.isEmpty()){
        ui->lineEditEip->setText((new QFileInfo(ingredientFile))->fileName());
    }
}

void FileSelection::on_pushButtonContinue_clicked()
{
    if(calculationFile.isEmpty() || ingredientFile.isEmpty()){
        QMessageBox::information(this,"Klaida","Nepasirinkti visi duomenų failai!");
    }else{
        if(this->close()){
            filesSelected = true;
        }
    }
}
