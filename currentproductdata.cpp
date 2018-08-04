#include "currentproductdata.h"

CurrentProductData::CurrentProductData()
{

}

void CurrentProductData::flushContainers()
{
    code = "";
    quantitiesNeeded.clear();
    quantitiesWarehouse.clear();
    quantitiesReservedOther.clear();
    quantitiesReservedThis.clear();
    currentCalculationCard.clear();
    allCodeVariants.clear();
}
