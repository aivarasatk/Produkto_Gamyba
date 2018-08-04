#include "productproduction.h"

#include <QDebug>
#include <QMessageBox>

#include <iomanip>
#include <iostream>
ProductProduction::ProductProduction(QWidget *mainWindow)
{
    //database setup
    refToWindow = mainWindow;
}

void ProductProduction::setCurrentCalculationCard(FileData &fileData, CurrentProductData& productData, QString code)
{
    const std::multiset<std::vector<QString>> &calculationCards = fileData.getCalculationCards();//gaunami visu produktu duomenys
    auto &currentCalculationCard = productData.getCurrentCalculationCard();//gaunama (tuscia) dabartine kortele
    bool found = false;

    for(auto &it:calculationCards)
    {
        if(it[(int)Line::PRODUCTCODE] == code)//tirinamas produkto kodas
        {
            currentCalculationCard.push_back(it);//sugeneruojam dabartine kalkuliacine kortele
            if(found == false && !it[(int)Line::COMPONENT1].isEmpty()){
                 found = true;
            }
        }
        else if(it[(int)Line::PRODUCTCODE] != code && found)//kai rasta preke ir nebelygi kodui nutrauksim cikla
            break;
    }
}

std::vector<double> ProductProduction::getProductNeeded(CurrentProductData& productData)
{
    auto currentCalculationCard = productData.getCurrentCalculationCard();

    std::vector<double> defaultCalculation;

    QString valueBefore = "";
    for(auto &it2:currentCalculationCard)
    {
        if(it2[(int)Line::COMPONENT1] != valueBefore)//it2[1] kodas
        {
            valueBefore = it2[(int)Line::COMPONENT1];//nustatom, kad neimtume to pacio kodo
            double numberValue = it2[(int)Line::AMOUNTMADEFROMCOMPONET].toDouble();//it2[7] kiekis
            defaultCalculation.push_back(numberValue);
            break;//suradom skaiciu, toliau ieskot nebereikia
        }
    }

    return defaultCalculation;
}

double ProductProduction::getDefaultCalculation(CurrentProductData& productData)
{
    std::vector<double> defaultCalculation = getProductNeeded(productData);

    double defaultCalc = 0;
    int globalCounter = 0;
    for(auto it:defaultCalculation)
    {
        int counter = 0;
        //skaiciuojam kiek koks kiekis pasikartoja
        for(auto it2: defaultCalculation)
        {
            if(it2 == it)
                counter++;
        }
        //atrenkam daugiausiai pasikartojanti kieki
        if(counter > globalCounter)
        {
            defaultCalc = it;
            globalCounter = counter;
        }
    }

    return defaultCalc;
}

void ProductProduction::setCoefficient(CurrentProductData& productData, double newCalculation)
{
    double defaultCalc = getDefaultCalculation(productData);

    if(defaultCalc == newCalculation || newCalculation == 0){
        coefficient = 1;
    }
    else{
        coefficient = newCalculation / defaultCalc;
    }
}

void ProductProduction::setProductCode(QString code)
{
    productCode = code;
}

bool ProductProduction::setProductName(QString code, FileData &fileData){
    const auto& calculationCards = fileData.getCalculationCards();
    for(auto it:calculationCards)
    {
            if(it[(int)Line::PRODUCTCODE] == code && it[(int)Line::COMPONENT1] == "" && it[(int)Line::COMPONENT2] == ""
               && it[(int)Line::COMPONENT3] == "")
            {
                  productName = it[(int)Line::NAME];//lokalus product name
                  return true;
            }
    }
    return false;
}


void ProductProduction::setIngredientsNeeded(CurrentProductData &productData)
{
    const auto &currentCalculationCard = productData.getCurrentCalculationCard();
    std::vector<double> &quantityNeeded = productData.getQuantitiesNeeded();

    for(auto it:currentCalculationCard)//randamas vardas pagal kalkuliacine lentele
    {
        QString value = it[(int)Line::COMPONENTREQUIRED].trimmed();
        if(!value.isEmpty()){
            double amountAdjustedToCoefficient = value.toDouble() * coefficient;
            quantityNeeded.push_back(amountAdjustedToCoefficient);
        }
    }
}

void ProductProduction::setIngredientsWarehouse(FileData &fileData, CurrentProductData& productData)
{
    setComponentRemainderByColumn((int)Line::COMPONENT3, fileData,productData);
    setComponentRemainderByColumn((int)Line::COMPONENT2, fileData,productData);
    setComponentRemainderByColumn((int)Line::COMPONENT1, fileData,productData);
}

void ProductProduction::setComponentRemainderByColumn(int columnIndex, FileData &fileData, CurrentProductData& productData){
    const auto currentCalculationCard = productData.getCurrentCalculationCard();
    const auto &ingredientsInfo = fileData.getIngredientsInfo();
    auto &quantitiesWarehouse = productData.getQuantitiesWarehouse();
    auto &allCodeVariants = productData.getAllCodeVariants();

    QString valueBefore = "";
    for(auto it:currentCalculationCard)
    {
        double sum = 0;
        QString code = it[columnIndex].trimmed();
        if(!code.isEmpty() && code != valueBefore){
            valueBefore = code;

            QString plainCode = modifyCodeForQuery(code);

            std::vector<QString> currVariants;
            currVariants.push_back(code);//idedam koda is lenteles, nes gali nebuti jokiu likuciu (tada nebus kodu)
            for(auto it2:ingredientsInfo)
            {
                if(it2[ (int)Eip::CODE ].contains(plainCode) && (it2[ (int)Eip::CODE ].size() == code.size()))
                {
                    if(it2[ (int)Eip::CODE ].size() > plainCode.size()){//tikrinam 0714AP = 07140A toki varianta (0714 abiejuose ir ilgis vienodas)
                        if(it2[ (int)Eip::CODE ][plainCode.size()] < '0' || it2[ (int)Eip::CODE ][plainCode.size()] > '9'){
                            addCodeVariant(it2[ (int)Eip::CODE ], currVariants);
                            sum += it2[ (int)Eip::AMOUNT ].toDouble() / it2[(int)Eip::FRACTION].trimmed().toInt();//kiekis / is frakcijos
                            continue;
                        }else{
                            continue;
                        }
                    }
                    addCodeVariant(it2[ (int)Eip::CODE ], currVariants);//FIXME
                    sum += it2[ (int)Eip::AMOUNT ].toDouble() / it2[(int)Eip::FRACTION].trimmed().toInt();// kiekis

                }
            }
            quantitiesWarehouse.push_back(sum);
            allCodeVariants.push_back(currVariants);
        }
    }
}

void ProductProduction::addCodeVariant(QString variant, std::vector<QString> &allVariants){
    bool variantExists = false;
    for(auto currVariant: allVariants){
        if(currVariant == variant){
           variantExists = true;
            break;
        }
    }
    if(!variantExists){
        allVariants.push_back(variant);
    }
}

void ProductProduction::setReservedQuantities(FileData &fileData, CurrentProductData& productData)
{
    getReservedQuantity((int)Line::COMPONENT3,fileData,productData);
    getReservedQuantity((int)Line::COMPONENT2,fileData,productData);
    getReservedQuantity((int)Line::COMPONENT1,fileData,productData);
}

void ProductProduction::getReservedQuantity(int componentIndex, FileData &fileData, CurrentProductData& productData)
{
    const auto currentCalculationCard = productData.getCurrentCalculationCard();
    const auto &ingredientsInfo = fileData.getIngredientsInfo();
    std::vector<double> &quantitiesReservedOther = productData.getQuantitiesReservedOther();
    std::vector<double> &quantitiesReservedThis = productData.getQuantitiesReservedThis();

    QString valueBefore = "";//netikrinsim tokiu paciu kodu, einanciu is eiles
    for(auto it: currentCalculationCard)
    {
        double sumThis = 0;
        double sumOther = 0;
        QString code = it[componentIndex].trimmed();//gaunam koda
        if(!code.isEmpty() && code != valueBefore){
            valueBefore = code;//nustatom nauja koda
            QString plainCode = modifyCodeForQuery(code);

            for(auto it2:ingredientsInfo){
                if(it2[ (int)Eip::CODE ].contains(plainCode) && (it2[ (int)Eip::CODE ].size() == code.size())
                    && !it2[ (int)Eip::OBJECT ].isEmpty())// //jei produktas turi objekta
                {
                    if(it2[ (int)Eip::CODE ].size() > plainCode.size()){//tikrinam 0714AP = 07140A toki varianta (0714 abiejuose ir ilgis vienodas)
                        if(it2[ (int)Eip::CODE ][plainCode.size()] < '0' || it2[ (int)Eip::CODE ][plainCode.size()] > '9'){

                            if( !it2[ (int)Eip::OBJECT ].contains(productCode) ||
                                (it2[ (int)Eip::OBJECT ].contains(productCode) &&
                                it2[ (int)Eip::OBJECT ].size() - 1 != productCode.size())
                                ){//jei ne siam produktui skirta
                                sumOther += it2[(int)Eip::AMOUNT].toDouble() / it2[(int)Eip::FRACTION].trimmed().toInt();//likutis / frakcijos
                            }else if(it2[ (int)Eip::OBJECT ].contains(productCode) &&
                                     it2[ (int)Eip::OBJECT ].size() - 1 == productCode.size()){//jei SIAM produktui skirta
                                sumThis += it2[(int)Eip::AMOUNT].toDouble() / it2[(int)Eip::FRACTION].trimmed().toInt();
                            }
                        }
                    }
                }
            }
            quantitiesReservedOther.push_back(sumOther);
            quantitiesReservedThis.push_back(sumThis);
        }
    }
}

void ProductProduction::sortFieldVectors(CurrentProductData& productData){
    auto itemIndexes = getItemIndexes(productData);

    auto &quantitiesWarehouse = productData.getQuantitiesWarehouse();
    auto &quantitiesReservedOther = productData.getQuantitiesReservedOther();
    auto &quantitiesReservedThis = productData.getQuantitiesReservedThis();

    sortVectorByIndexes(itemIndexes, quantitiesWarehouse);
    sortVectorByIndexes(itemIndexes, quantitiesReservedOther);
    sortVectorByIndexes(itemIndexes, quantitiesReservedThis);

}

std::vector<int> ProductProduction::getItemIndexes(CurrentProductData& productData){
    const auto currentCalculationCard = productData.getCurrentCalculationCard();
    std::vector<int> componentIndexes;
    std::vector<int> itemIndexes;

    componentIndexes.push_back((int)Line::COMPONENT3);
    componentIndexes.push_back((int)Line::COMPONENT2);
    componentIndexes.push_back((int)Line::COMPONENT1);

    for(auto componentIndex:componentIndexes){
        int lineIndex = 0;
        QString valueBefore = "";
        for(auto line: currentCalculationCard){
            if(!line[componentIndex].isEmpty() && line[componentIndex] != valueBefore){
                itemIndexes.push_back(lineIndex);
                valueBefore = line[componentIndex];
            }
            ++lineIndex;
        }
    }
    return itemIndexes;
}

QString ProductProduction::modifyCodeForQuery(const QString &code)
{
    QString final = code;
    int length = final.length();

    //jei kodas jau yra plikas (be raidziu gale) ieskom be wildcards
    if(final[length-1] >= '0' && final[length-1] <='9'){
        return final;
    }
    else if(final[length-1] == 'B' && (final[length-2] >= '0' && final[length-2] <= 9) ){//gaminamas produktas bazej yra pilnu pav
        return final;
    }
    else//nuimsim nereikalingas raides
    {
        int counter = 0;// kiek raidziu nuimam
        for(int i = length-1; ; --i)
        {
            if(final[i] >= '0' && final[i] <='9')
                break;
            else
            {
                final.remove(i,1);
                ++counter;
            }
        }
        return final.trimmed();
    }
}

void ProductProduction::recalculateQuantitiesWarehouse(CurrentProductData& current, std::vector<CurrentProductData>& previous, uint currentIndex){
    //perskaiciuojam likucius esamam elementui, jei randam tu paciu kodu pagal buvusius likucius/reikalavimus
    auto& currentCalculationCard = current.getCurrentCalculationCard();
    auto& currentQuantitiesWarehouse = current.getQuantitiesWarehouse();
    std::vector<QString> checkedProducts;

    int indexesSize = 3;
    int componentIndexes[indexesSize] = {(int)Line::COMPONENT1, (int)Line::COMPONENT2, (int)Line::COMPONENT3};

    for(int h = currentIndex-1; h >=0; --h){
        if(productNotChecked(checkedProducts, previous[h].getCode())){
            checkedProducts.push_back(previous[h].getCode());
            for(int index = 0; index < indexesSize; ++index){
                QString valueBefore = "";
                for(uint i = 1; i < currentCalculationCard.size(); ++i){//eisim per visas dabartinius produktus
                    if(!currentCalculationCard[i][componentIndexes[index]].isEmpty()
                            && currentCalculationCard[i][componentIndexes[index]].trimmed() != valueBefore){//nevykdom tustiem ir pasikartojantiem elementam

                        valueBefore = currentCalculationCard[i][componentIndexes[index]].trimmed();
                        double whatsLeft = 0;//paskutinis likutis; koreguosim funkcijoj
                        if(isInPreviousProduct(previous[h],currentCalculationCard[i][componentIndexes[index]].trimmed(), whatsLeft) ){
                            currentQuantitiesWarehouse[i-1] = whatsLeft;
                        }
                    }

                }
            }
        }

    }

}

bool ProductProduction::isInPreviousProduct(CurrentProductData& previous, QString currentCode, double& whatsLeft){
    int indexesSize = 3;
    int componentIndexes[indexesSize] = {(int)Line::COMPONENT1, (int)Line::COMPONENT2, (int)Line::COMPONENT3};

    auto& previousCalculationCard = previous.getCurrentCalculationCard();
    auto& previousQuantitiesWarehouse = previous.getQuantitiesWarehouse();
    auto& previousReservedThis = previous.getQuantitiesReservedThis();
    auto& previousReservedOther = previous.getQuantitiesReservedOther();

    for(int i = 0; i < indexesSize; ++i){
        for(uint j = 1; j < previousCalculationCard.size(); ++j){
            QString thisCode = previousCalculationCard[j][componentIndexes[i]].trimmed();
            if(currentCode == thisCode){
                whatsLeft = previousQuantitiesWarehouse[j-1] - previousCalculationCard[j][(int)Line::COMPONENTREQUIRED].toDouble();
                whatsLeft = whatsLeft - previousReservedThis[j-1] -  previousReservedOther[j-1];
                if(whatsLeft < 0){
                    whatsLeft = previousQuantitiesWarehouse[j-1];
                }
                return true;
            }
        }
    }
    return false;
}

bool ProductProduction::productNotChecked(std::vector<QString>& codeList, QString code){
    for(auto it: codeList){
        if(it == code){
            return false;
        }
    }
    return true;
}
