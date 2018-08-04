#ifndef FILESELECT_H
#define FILESELECT_H
#include <QString>
#include <QWidget>

class FileSelect
{
public:
    FileSelect();
    void selectFile(QWidget *window, QString format);//failo pasirinkimas
    QString getName() {return selectedFile;}
private:
    QString selectedFile;
    QString currentDirectory;
};

#endif // FILESELECT_H
