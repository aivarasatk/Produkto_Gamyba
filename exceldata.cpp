#include "exceldata.h"

#include <xlsxdocument.h>
#include <QDebug>
#include <iostream>
#include <fstream>
ExcelData::ExcelData(QString inputFileName, std::multiset<std::vector<QString>> &inputContainer):
    fileName(inputFileName),container(inputContainer)
{

}

void ExcelData::parseExcelFile(const std::vector<int> &columnsToRead, int startAt, const std::vector<int> &parseNumsAt, bool addName)
{
    QXlsx::Document xlsx(fileName);
    int arraySize = columnsToRead.size();

    while(1)
    {
        std::vector<QString> currData;
        QString product = xlsx.read(startAt,columnsToRead[0]).toString();
        if(product != "")
        {
            QString code, name;
            separateCodeName(product,code, name);
            currData.push_back(code);

            if(addName)
                currData.push_back(name);

            //get remaining row
            for(int i = 1; i<arraySize; ++i)// gaunama visas eiles duomenu sarasas
            {
                QString tempData = xlsx.read(startAt,columnsToRead[i]).toString();
                tempData = tempData.trimmed();

                parseNumbers(i, parseNumsAt, tempData);

                currData.push_back(tempData);
            }
            container.insert(currData);

            ++startAt;
        }
        else
            break;
    }

    if(container.size() == 0)
    {
        std::string error = "Duomenų failas tuščias arba neegzistuoja";
        throw error;
    }
}

void ExcelData::separateCodeName(const QString &product, QString &code, QString &name)
{
    bool codeDone = false;
    for(int i = 0; i<product.size(); ++i)
    {
        if(product[i] != ' ' && !codeDone)
        {
            code+=product[i];
        }
        else if(product[i]==' ' && !codeDone)
            codeDone = true;
        else if(codeDone)
        {
            name += product[i];
        }
    }
    name = name.trimmed();
}

void ExcelData::parseNumbers(int currIndex, const std::vector<int> &parseNumsAt, QString &tempData)
{
    //parse numbers
    for(auto index:parseNumsAt)
    {
        if(currIndex == index)
        {
            QString code;
            //gaunamas tik kiekis, be mat. vnt.
            for(int i = 0; tempData[i] != ' ' && tempData[i]!='\0'; ++i)
            {
                code+=tempData[i];
            }
            tempData = code;
        }
    }
}
