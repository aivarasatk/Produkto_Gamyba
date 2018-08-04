#ifndef PRODUCTFIELDS_H
#define PRODUCTFIELDS_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
class ProductFields
{
private:
    void cleanUp(QLayout *layout);
public:
    ProductFields(bool deletable);
    ~ProductFields();

    QGridLayout* itemBlock;

    QLabel* productCodeText;
    QLineEdit* productCodeInput;
    QLabel* productNameText;
    QLineEdit* productNameOutput;

    QLabel* productAmountText;
    QLineEdit* productAmountInput;

    QCheckBox* checkBox;

    QGridLayout* getBlock();

};

#endif // PRODUCTFIELDS_H
