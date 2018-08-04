#include "eipfilehandler.h"

#include <QFileDialog>
#include <QDebug>
#include <fstream>
#include <string>
EipFileHandler::EipFileHandler()
{
    currentDirectory = QDir::currentPath();
}

bool EipFileHandler::readHowManyLinesToSkip(QString fileName, FileData &containers){
    std::ifstream read(fileName.toStdString());
    if(!read.is_open()){
        return false;
    }
    std::string tempBuff;

    std::vector<int> I07SkipData;
    std::vector<int> parseNumberAt;

    getline(read, tempBuff);            //failo pradzioje siuksles

    do{
        bool parseI07 = false;//ar skaiciaus laukas
        std::string number;
        if(getline(read, number)){

            QString qNumber = QString::fromStdString(number);
            int realNumber = -1;//konvertuotas skaicius is string
            if(qNumber[qNumber.length()-1] != 'n'){
                realNumber = qNumber.toInt();

            }else if(qNumber[qNumber.length()-1] == 'n'){//indikatorius kad laukas bus skaicius
                qNumber.remove(qNumber.length()-1,1);
                realNumber = qNumber.toInt();
                parseI07 = true;//isiminsim kad reikes versti i skaiciu
            }

            I07SkipData.push_back(realNumber);
            if(parseI07){//jei skaitinis laukas, reikes perdirbti i skaiciu; isimenam kelintas elementas
                parseNumberAt.push_back(I07SkipData.size()-1);
            }
      }

    }while(read);

    containers.setI07SkipData(I07SkipData);
    containers.setParseNumberAt(parseNumberAt);

    read.close();
    return true;
}

bool EipFileHandler::readEipFile(QString fileName, FileData& containers){

    auto &container = containers.getIngredientsInfo();
    std::ifstream file(fileName.toStdString());
    if(!file.is_open()){
        return false;
    }

    std::vector<int> I07SkipData = containers.getI07SkipData();

    while(file){
        std::vector<QString> blockData = readBlock(I07SkipData,file);
        if(blockData.size() != 0){
            container.insert(blockData);
        }else{
            break;
        }

    }
    return true;
}
std::vector<QString> EipFileHandler::readBlock(std::vector<int>& skipData, std::ifstream &file){

    std::vector<QString> extractedStrings;
    for(uint j = 0; j<skipData.size()-1; ++j){
        skipLines(skipData[j], file);
        std::string someString;
        getline(file,someString);
        if(!file){//baigesi failas nutraukiam darba
            extractedStrings.clear();//ne pilnai uzpildyta duomenimis, todel pilnai isvalom ir baigiam darba
            return extractedStrings;
        }
        QString parsedLine = parseLine(QString::fromStdString(someString));
        //qDebug() << parsedLine;
        extractedStrings.push_back(parsedLine);

    }

    skipLines(skipData[skipData.size()-1], file);//skipinam iki bloko pabaigos
    return extractedStrings;
}

QString EipFileHandler::parseLine(QString line){

    int closingBracketIndex = line.indexOf('>');
    int secondStartingBracketIndex = line.indexOf('<',closingBracketIndex+1);

    QString result;
    for(int i = closingBracketIndex+1; i<secondStartingBracketIndex; ++i){
        result += line[i];
    }
    result = result.trimmed();
    return result;
}

void EipFileHandler::skipLines(int skips, std::ifstream& file){

    for(int i=0; i<skips; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
