#ifndef PRODUCTPRODUCTION_H
#define PRODUCTPRODUCTION_H

#include <vector>
#include <QString>
#include <map>
#include <QSqlQuery>
#include <QMainWindow>
#include <QDebug>

#include "filedata.h"
#include "currentproductdata.h"

class MainWindow;

class ProductProduction
{
public:
    ProductProduction(QWidget *mainWindow);

    void setCurrentCalculationCard(FileData &fileData, CurrentProductData& productData, QString code);

    std::vector<double> getProductNeeded(CurrentProductData& productData);
    double getDefaultCalculation(CurrentProductData& productData);
    void setCoefficient(CurrentProductData& productData, double newCalculation);

    void setIngredientsNeeded(CurrentProductData &productData);
    void setIngredientsWarehouse(FileData &fileData, CurrentProductData& productData);
    void setComponentRemainderByColumn(int columnIndex, FileData &fileData, CurrentProductData &productData);
    void addCodeVariant(QString variant, std::vector<QString> &allVariants);


    void setReservedQuantities(FileData &fileData, CurrentProductData &productData);
    void getReservedQuantity(int componentIndex, FileData &fileData, CurrentProductData &productData);

    void sortFieldVectors(CurrentProductData &productData);
    template<class T> void sortVectorByIndexes(std::vector<int> itemIndexes, std::vector<T>& itemVector);//vector template
    std::vector<int> getItemIndexes(CurrentProductData& productData);
    template<class T> void swap(std::vector<T>& items, int i, int j);//vector template

    void setProductCode(QString code);
    bool setProductName(QString code, FileData &fileData);
    QString getProductCode() {return productCode;}
    QString getProductName() {return productName;}

    QString modifyCodeForQuery(const QString &code);

    void recalculateQuantitiesWarehouse(CurrentProductData& current, std::vector<CurrentProductData> &previous, uint currentIndex);
    bool isInPreviousProduct(CurrentProductData& previous, QString currentCode, double &whatsLeft);
    bool productNotChecked(std::vector<QString>& codeList, QString code);

    enum class Line{ PRODUCTCODE = 0, COMPONENT1 = 1, COMPONENT2 = 2, COMPONENT3 = 3,
              NAME = 4, COMPONENTREQUIRED = 5, COMPONENTMESSURINGUNIT = 6, AMOUNTMADEFROMCOMPONET = 7,
              RESULTMESSURINGUNIT = 8};

private:
    QWidget *refToWindow;

    enum class Eip{DEPARTMENT = 0, CODE = 1, OBJECT = 2, SERIALNUMBER = 3, FRACTION = 4, AMOUNT = 5};
    QString productName;
    QString productCode;
    double coefficient;
};


template<class T>void ProductProduction::sortVectorByIndexes(std::vector<int> itemIndexes, std::vector<T>& itemVector){

    for(uint i=0; i < itemIndexes.size(); ++i){
        for(uint j = i; j < itemIndexes.size(); ++j){
            if(itemIndexes[i] > itemIndexes[j]){
                swap(itemIndexes, i, j);
                swap(itemVector, i, j);
            }
        }
    }
}

template<class T> void ProductProduction::swap(std::vector<T>& items, int i, int j){
    T tempItem = items[i];
    items[i] = items[j];
    items[j] = tempItem;
}

#endif // PRODUCTPRODUCTION_H
