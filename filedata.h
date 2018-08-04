#ifndef FILEDATA_H
#define FILEDATA_H

#include <vector>
#include <set>
#include <QString>

class FileData{
public:
    FileData();
    std::multiset<std::vector<QString>>& getIngredientsInfo() {return ingredientsInfo;}
    std::multiset<std::vector<QString>>& getCalculationCards() {return calculationCards;}

    //eip stuff
    void setI07SkipData(std::vector<int>& I07Data) {I07SkipData = I07Data;}
    std::vector<int>& getI07SkipData() {return I07SkipData;}

    void setParseNumberAt(std::vector<int>& parseNumbers) {parseNumberAt = parseNumbers;}
    std::vector<int>& getParseNumbersAt() {return parseNumberAt;}
private:
    std::multiset<std::vector<QString>> ingredientsInfo;
    std::multiset<std::vector<QString>> calculationCards;

    //eip stuff
    std::vector<int> I07SkipData;
    std::vector<int> parseNumberAt;

};

#endif // FILEDATA_H
