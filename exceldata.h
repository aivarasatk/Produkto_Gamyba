#ifndef EXCELDATA_H
#define EXCELDATA_H

#include <QString>
#include <vector>
#include <set>

class ExcelData
{
public:
    ExcelData(QString inputFileName, std::multiset<std::vector<QString> > &inputContainer);

    void parseExcelFile(const std::vector<int> &columnsToRead, int startAt, const std::vector<int> &parseNumsAt, bool addName);
    void separateCodeName(const QString &product, QString &code, QString &name);
    void parseNumbers(int currIndex, const std::vector<int> &parseNumsAt, QString &tempData);



private:
    QString fileName;
    std::multiset<std::vector<QString>> &container;
};

#endif // EXCELDATA_H
