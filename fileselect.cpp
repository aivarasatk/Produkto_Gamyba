#include "fileselect.h"

#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
FileSelect::FileSelect()
{
    currentDirectory = QDir::currentPath();
}

void FileSelect::selectFile(QWidget *window, QString format){
    selectedFile = QFileDialog::getOpenFileName(window, "Atidarykit failą", currentDirectory, format);
    if(!selectedFile.isEmpty())
    {
        QFileInfo fileLocation = QFileInfo(selectedFile);
        currentDirectory = fileLocation.absolutePath();
    }
}
