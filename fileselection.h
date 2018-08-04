#ifndef FILESELECTION_H
#define FILESELECTION_H

#include <QDialog>
#include <QRunnable>

#include "exceldata.h"
#include "fileselect.h"
#include "progressbar.h"
#include "filedata.h"

namespace Ui {
class FileSelection;
}

class FileSelection : public QDialog, public QRunnable
{
    Q_OBJECT

public:
    FileSelection(QWidget *parent = 0, FileData &containers = *(new FileData()) );
    ~FileSelection();

    void prepareDataFiles();
    bool getFilesSelected() {return filesSelected;}

    void setProgressBar(ProgressBar* orgProgressBar){progressBar = orgProgressBar;}

    void run(){prepareDataFiles();}

private slots:
    void on_pushButtonExcel_clicked();

    void on_pushButtonEip_clicked();

    void on_pushButtonContinue_clicked();

private:
    Ui::FileSelection *ui;
    FileData &orgContainers;
    FileSelect fileSelect;
    ProgressBar* progressBar;
    QString calculationFile, ingredientFile;
    bool filesSelected;
    void prepareDataFile(QString fileName, std::multiset<std::vector<QString>> &container,
                         std::vector<int> columnsToRead, std::vector<int> parseNumsAt, int startAt);
    void prepareCalculationCardFile(std::vector<int> &columnsToRead, std::vector<int> &parseNumsAt);
};

#endif // FILESELECTION_H
