#ifndef EIPFILEHANDLER_H
#define EIPFILEHANDLER_H

#include "filedata.h"

#include <QString>
#include <fstream>
class EipFileHandler
{
public:
    EipFileHandler();

    //.eip likuciam nuskaityt
    bool readHowManyLinesToSkip(QString fileName, FileData &containers);
    bool readEipFile(QString fileName, FileData &containers);
    std::vector<QString> readBlock(std::vector<int>& skipData, std::ifstream &file);
    QString parseLine(QString line);
    void skipLines(int skips, std::ifstream& file);
private:
    QString currentDirectory;

};

#endif // EIPFILEHANDLER_H
