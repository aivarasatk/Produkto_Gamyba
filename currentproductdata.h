#ifndef CURRENTPRODUCTDATA_H
#define CURRENTPRODUCTDATA_H


#include <vector>
#include <set>
#include <QString>

class CurrentProductData
{
public:
    CurrentProductData();
    void flushContainers();
    std::vector<std::vector<QString>>& getCurrentCalculationCard() {return currentCalculationCard;}

    std::vector<double>& getQuantitiesWarehouse() {return quantitiesWarehouse;}
    std::vector<double>& getQuantitiesNeeded() {return quantitiesNeeded;}
    std::vector<double>& getQuantitiesReservedOther() {return quantitiesReservedOther;}
    std::vector<double>& getQuantitiesReservedThis() {return quantitiesReservedThis;}


    std::vector<std::vector<QString>>& getAllCodeVariants(){return allCodeVariants;}

    QString& getCode() {return code;}

    void setQuantitiesWarehouse(std::vector<double>& newQW) {quantitiesWarehouse = newQW;}
    void setQuantitiesNeeded(std::vector<double>& newQN) {quantitiesNeeded = newQN;}
    void setquantitiesReservedOther(std::vector<double>& newQR) {quantitiesReservedOther = newQR;}
    void setquantitiesReservedThis(std::vector<double>& newQR) {quantitiesReservedThis = newQR;}

    void setCode(QString newCode) {code = newCode;}
private:
    QString code;
    std::vector<double> quantitiesWarehouse;
    std::vector<double> quantitiesNeeded;
    std::vector<double> quantitiesReservedOther;
    std::vector<double> quantitiesReservedThis;
    std::vector<std::vector<QString>> currentCalculationCard;

    std::vector<std::vector<QString>> allCodeVariants;//produkto ingridientu kodu variantai

};

#endif // CURRENTPRODUCTDATA_H
