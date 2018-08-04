#include "productfields.h"

#include <QDebug>
ProductFields::ProductFields(bool deletable)
{
    itemBlock = new QGridLayout();

    //** 3 ***///istrynimo checkbox
    checkBox = new QCheckBox();
    itemBlock->addWidget(checkBox,0,0,2,1);
    if(!deletable){
        checkBox->setEnabled(false);
    }

    /*** 1 ***/
    productCodeText = new QLabel("Ieškomos prekės kodas:");
    productCodeInput = new QLineEdit();
    /* add */
    itemBlock->addWidget(productCodeText,0,1);
    itemBlock->addWidget(productCodeInput,0,2);

    productNameText = new QLabel("Pavadinimas:");
    productNameOutput = new QLineEdit();
    productNameOutput->setEnabled(false);

    QFont bold;
    bold.setBold(true);
    productNameOutput->setFont(bold);

    /* add */
    itemBlock->addWidget(productNameText, 0, 3);
    itemBlock->addWidget(productNameOutput, 0, 4);

    /*** 2 ***/
    productAmountText = new QLabel("Gaminamas kiekis:");
    productAmountInput = new QLineEdit();
    productAmountInput->setPlaceholderText("*Nieko neįvedus imami kalkuliacinės kortelės kiekiai*");
    itemBlock->addWidget(productAmountText, 1, 1);
    itemBlock->addWidget(productAmountInput, 1, 2);


}

ProductFields::~ProductFields(){

    delete productCodeText;
    delete productCodeInput;
    delete productNameText;
    delete productNameOutput;
    delete productAmountText;
    delete productAmountInput;
    delete checkBox;

    cleanUp(itemBlock);//still not clear why the error
    delete itemBlock;
}

QGridLayout* ProductFields::getBlock(){
    return itemBlock;
}

void ProductFields::cleanUp(QLayout* layout){
    QLayoutItem *child;
    QLayout * sublayout;
    QWidget * widget;
    while ((child = layout->takeAt(0))) {
        if ((sublayout = child->layout()) != 0) { cleanUp(sublayout);}
        else if ((widget = child->widget()) != 0) {widget->hide(); delete widget;}
        else {delete child;}
    }
}
