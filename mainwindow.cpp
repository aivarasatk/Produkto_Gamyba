#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "eipfilehandler.h"
#include "productfields.h"

#include <QDebug>
#include <QTableWidget>
#include <QHeaderView>
#include <QFont>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QThreadPool>
#include <QSignalMapper>

#include <iostream>
#include <vector>
#include <iomanip>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), vWindowLayout(new QVBoxLayout()),
    pp(ProductProduction(parent)),
    fileSelection(new FileSelection(this, fileData)),
    progressBar(new ProgressBar)
{
    ui->setupUi(this);
    setWindowTitle("Produkto žaliavų tikrinimas");

    QWidget* mainLayout = new QWidget();
    mainLayout->setLayout(vWindowLayout);
    setCentralWidget(mainLayout);

    setMainWidgets();

    //ui stuff
    ui->tableWidget->setWordWrap(true);//PAKEIST KODU
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//neleidziam redaguot lenteles

    connect(ui->tableWidget->horizontalHeader(), SIGNAL(sectionResized(int,int,int)),
            ui->tableWidget, SLOT(resizeRowsToContents()));//keiciant stulepeliu dydi, keiciasi eiluciu dydis

    connect(ui->tableWidget->horizontalHeader(), SIGNAL(sectionResized(int,int,int)),
            this, SLOT(setWindowWidthToMatchTable()));//dinamiskai keiciamas lango dydis
}

MainWindow::~MainWindow()
{
    delete ui;
}

 void MainWindow::insertItemToTable(int row, int &column, QString item)
 {
     ui->tableWidget->setItem(row, column, new QTableWidgetItem(item));
     ++column;
 }

void MainWindow::displayListTableModel()
{

    //nustatoma horizontalus lenteles stulpeliu pav.
    std::vector<QString> headerItems;
    setHeaderNames(headerItems);

    ui->tableWidget->setColumnCount(headerItems.size());
    setTableRowCount();
    setHeaderItems(headerItems);


    int currentRow = 0;
    for(uint index = 0; index < allProductData.size(); ++index){
        if(index != 0){
            pp.recalculateQuantitiesWarehouse(allProductData[index], allProductData, index);
        }
        //paruosiami konteineriai
        const std::vector<double> &quantityNeeded = allProductData[index].getQuantitiesNeeded();
        const std::vector<double> &quantityWarehouse = allProductData[index].getQuantitiesWarehouse();
        const std::vector<double> &quantityReservedOther = allProductData[index].getQuantitiesReservedOther();
        const std::vector<double> &quantityReservedThis = allProductData[index].getQuantitiesReservedThis();

        setComponentColumns(allProductData[index], currentRow);//prekiu pavadininu laukai

        for(uint i = 0; i <quantityNeeded.size(); ++i){
            int currentColumn = 4;//po 3 komponenciu ir pavadinimu

            double reserved = quantityReservedOther[i] + quantityReservedThis[i];//kiek bendrai rezervuota
            double finalQuantity = quantityWarehouse[i] - reserved;

            //nustatomi kiekiai apie preke
            insertNumberValue(i + currentRow, currentColumn, quantityNeeded[i]);//reikalingas kiekis gaminimui
            insertNumberValue(i + currentRow, currentColumn, finalQuantity);//laisvas likutis
            insertNumberValue(i + currentRow, currentColumn, quantityReservedThis[i]);//kiek rezervuota siam produktui
            insertNumberValue(i + currentRow, currentColumn, quantityReservedOther[i]);//kiek rezervuota kitiems produktams

            if(quantityNeeded[i] <= finalQuantity){
                insertItemToTable(i + currentRow,currentColumn, "Užtenka");//taip
                ui->tableWidget->item(i + currentRow,currentColumn-1)->setBackgroundColor(Qt::green);
            }else if(quantityNeeded[i] > finalQuantity) {
                insertItemToTable(i + currentRow,currentColumn,"Neužtenka");//ne
                ui->tableWidget->item(i + currentRow,currentColumn-1)->setBackgroundColor(Qt::red);
            }
        }
        currentRow += quantityNeeded.size();//perstumiam eiltes per produkta
        separateLines(currentRow);
        ++currentRow;//atskiriam duomenis linija
        ui->tableWidget->resizeColumnsToContents();
    }
}

void MainWindow::setComponentColumns(CurrentProductData &containers, int currentRow){
    auto allCodeVariants = containers.getAllCodeVariants();
    auto currentCalculationCard = containers.getCurrentCalculationCard();
    int componentIndexes[] = {(int)pp.Line::COMPONENT3,(int)pp.Line::COMPONENT2,(int)pp.Line::COMPONENT1};
    int size = 3;

    int counter = 0;
    int nameCounter = 0;
    for(int i=0; i < size; ++i){
        int emptyCounter = 0;
        int notEmptyCounter = 0;

        QString valueBefore = "";
        bool skipFirstLine = true;


        for(auto item: currentCalculationCard){
            if(skipFirstLine){//praskipinam pirma eilute
                skipFirstLine = false;
                continue;
            }

            QString currentCode = item[componentIndexes[i]].trimmed();
            if(!currentCode.isEmpty() && currentCode != valueBefore){//nustatomi visi kodu variantai
                valueBefore = currentCode;
                addCodeVariantsToTable(notEmptyCounter + emptyCounter + currentRow, size - i - 1, allCodeVariants[counter]);
                ++notEmptyCounter;
                ++counter;
            }else{//nustatomi paprasti kodai (be variantu)
                ui->tableWidget->setItem(notEmptyCounter + emptyCounter + currentRow, size - i - 1, new QTableWidgetItem(currentCode));
                ++emptyCounter;
            }
            if(i == 0){//nustatomi produktu pavadinimai
                ui->tableWidget->setItem(nameCounter + currentRow, 3, new QTableWidgetItem(item[(int)pp.Line::NAME]));
                ++nameCounter;
            }
        }
    }
}

void MainWindow::setHeaderNames(std::vector<QString>& headerItems){
    for(int i = 0; i<3; ++i){
        headerItems.push_back("Komponente " + QString::number(i+1));
    }
    headerItems.push_back("Pavadinimas");
    headerItems.push_back("Poreikis");
    headerItems.push_back("Laisvas\nlikutis");
    headerItems.push_back("Rezervuota\nšiam\nproduktui");
    headerItems.push_back("Rezervuota\nkitiems\ngaminiams");
    headerItems.push_back("Statutas");
}

void MainWindow::setHeaderItems(std::vector<QString> &headerItems)
{
    for(uint i= 0; i<headerItems.size(); ++i)
    {
        ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(headerItems[i]));
    }
    int columnWidth = ui->tableWidget->columnWidth(0);
    ui->tableWidget->horizontalHeaderItem(0)->setSizeHint(QSize(columnWidth, 55));
}

void MainWindow::on_search_button_clicked()
{
    //isvaloma sena informacija
    ui->tableWidget->clear();

    for(auto item : productFields){
        allProductData.push_back(CurrentProductData());//yra naujas produkto laukas, idedam duomenu lauka

        //jei neteisingai nustatymas gaminamas kiekis
        QRegExp re("\\d*");  // a digit (\\d), zero or more times (*)
        if (!re.exactMatch(item->productAmountInput->text().trimmed())){
            QMessageBox::information(this,"Klaida", "Neteisingas gaminamas kiekis");
            allProductData.clear();
            return;
        }

        //gaunamas produkto kodas
        QString code = item->productCodeInput->text().trimmed();
        //jei kodas tuscias
        if(!pp.setProductName(code, fileData)){
            QMessageBox::critical(this, "Error", "Nerasta kalkuliacinė lentelė produktui: " + code);
            allProductData.clear();
            return;
        }
        else{
            pp.setProductCode(code);
            allProductData.back().setCode(code);

            //gaunami pagrindiniai (be sudedamuju daliu) komponentai
            pp.setCurrentCalculationCard(fileData, allProductData.back(), code);

            //nustatomas koeficientas, kuris lemia zaliavu kiekiu perskaiciavima
            pp.setCoefficient(allProductData.back(), item->productAmountInput->text().toDouble());

            //gaunami zaliavu kiekiai
            pp.setIngredientsNeeded(allProductData.back());//nustatom pagal koeficienta
            pp.setIngredientsWarehouse(fileData, allProductData.back());//gaunam likucius

            pp.setReservedQuantities(fileData, allProductData.back());//kiek kam rezervuota

            pp.sortFieldVectors(allProductData.back());
            //rodoma lentele

        }
    }

    displayListTableModel();
    paintTable();
    setWindowWidthToMatchTable();

    allProductData.clear();//isvalom duomenu vektoriu
}
void MainWindow::on_product_name_changed(ProductFields* productField){
    productField->productCodeInput->blockSignals(true);//bug workaround (enter key emits signal twice)

    uint codeIndex = productBlockIndexes.find(productField)->second;//randam bloko indeksa
    if(productField->productCodeInput->text().trimmed() != productCodes[codeIndex]){
        productCodes[codeIndex] = productField->productCodeInput->text().trimmed();

        if(pp.setProductName(productCodes[codeIndex], fileData)){
            productField->productNameOutput->setText(pp.getProductName());//nustatom ui koda
        }else{
            productField->productNameOutput->setText("*Nerasta*");
        }
    }

    productField->productCodeInput->blockSignals(false);
}

void MainWindow::setFocusOnAmountLineEdit(ProductFields* productField){

    productField->productCodeInput->blockSignals(true);//bug workaround (enter key emits signal twice)
    productField->productAmountInput->setFocus();
    productField->productCodeInput->blockSignals(false);
}

//produkto sudeties kodai
void MainWindow::putCodesToTable(CurrentProductData &containers)
{
    const auto &currentCalculationCard = containers.getCurrentCalculationCard();

    int row = 0;
    for(auto it: currentCalculationCard)
    {
        int column = 0;
        if(!it[1].isEmpty())
        {
            ui->tableWidget->setItem(row, column, new QTableWidgetItem(it[1]));
            ui->tableWidget->setItem(row, column+1,  new QTableWidgetItem(it[2]));
            ui->tableWidget->setItem(row, column+2,  new QTableWidgetItem(it[3]));
        }
        ++row;
    }
}

void MainWindow::setTableRowCount(){
    int rowCount = 0;
    for(auto item: allProductData){
        const std::vector<double> &quantityNeeded = item.getQuantitiesNeeded();
        rowCount += quantityNeeded.size();
    }
    ui->tableWidget->setRowCount(rowCount + allProductData.size() - 1);
}

void MainWindow::setWindowWidthToMatchTable(){
    int tableWidth = 0;
    tableWidth += ui->tableWidget->contentsMargins().left()
            + ui->tableWidget->contentsMargins().right();
    tableWidth += ui->tableWidget->verticalHeader()->width();
    for (int i=0; i<ui->tableWidget->columnCount(); ++i)
        tableWidth += ui->tableWidget->columnWidth(i);

    this->setMinimumWidth(tableWidth + 55);
    this->setMaximumWidth(tableWidth + 55);
}

void MainWindow::insertNumberValue(int row, int &column,double quantity){
    if((int)quantity != quantity) {//sutvarkom skaicius po kablelio
        insertItemToTable(row, column, QString::number(quantity, 'f', 3));
    }else{
        insertItemToTable(row, column, QString::number(quantity, 'f', 0));
    }
}
void MainWindow::addCodeVariantsToTable(int row, int column,std::vector<QString> &codeVariants){
    QString formedString = formTableItem(codeVariants);
    ui->tableWidget->setItem(row,column, new QTableWidgetItem(formedString));
}

QString MainWindow::formTableItem(const std::vector<QString> &codeVariants){
    QString formedItem = "";
    for(auto code: codeVariants){
        formedItem += code;
        formedItem += '\n';
    }
    formedItem.remove(formedItem.size()-1, 1);//nuimam paskitni \n
    return formedItem;
}

void MainWindow::showFileSelection(){
    fileSelection->setModal(true);
    fileSelection->exec();
    if(!fileSelection->getFilesSelected()){//jei uzdaromas langas, baigiam programa
        exit(1);
    }else{
        fileSelection->setAutoDelete(false);
        fileSelection->setProgressBar(progressBar);

        connect(progressBar, SIGNAL(filesProcessed()),
                this, SLOT(closeProgressBar()));
        progressBar->show();//rodom failu apdorojimo progresa
        QThreadPool::globalInstance()->start(fileSelection);
    }
}

void MainWindow::closeProgressBar(){
    QThread::msleep(200);
    progressBar->close();
    delete progressBar;
    delete fileSelection;
}

void MainWindow::paintTable(){
    int rowCount = ui->tableWidget->rowCount();
    int lastColumnToPaint = (int)pp.Line::COMPONENTREQUIRED;

    QColor teal(204,255,255);
    for(int i = 0; i< rowCount; ++i){//nustatom visus laukus pirma spalva (reikiamus perspalvinsim)
        for(int j = 0; j < lastColumnToPaint; ++j){
            if(!ui->tableWidget->item(i,0)->text().isEmpty()){
                ui->tableWidget->item(i,j)->setBackgroundColor(teal);
            }
        }
    }
    int component2Index = 1;
    QColor greenish(153,204,0);
    for(int i = 0; i< rowCount; ++i){//perspalvinam antras komponenetes
        if(!ui->tableWidget->item(i,component2Index)->text().isEmpty()){
            for(int j = component2Index; j < lastColumnToPaint; ++j){
                ui->tableWidget->item(i,j)->setBackgroundColor(greenish);
            }
        }
    }

    int component3Index = 2;
    QColor yellow(255,255,0);
    for(int i = 0; i< rowCount; ++i){//perspalvinam trecias komponentes
        if(!ui->tableWidget->item(i,component3Index)->text().isEmpty()){
            for(int j = component3Index; j < lastColumnToPaint; ++j){
                    ui->tableWidget->item(i,j)->setBackgroundColor(yellow);
            }
        }
    }
}
void MainWindow::addProductFields(bool deletable){
    ProductFields* productField = new ProductFields(deletable);

    productCodes.push_back("");//padidinam masyva elementu
    productBlockIndexes.insert(
                std::pair<ProductFields*, uint>(productField, productFields.size()));//isimenam bloko indeksa

    productFields.push_back(productField);//isimenam bloko adresa

    vWindowLayout->insertLayout(productFields.size()-1,productFields.back()->getBlock());//idedam nauja bloka

    connect(productField->productCodeInput, &QLineEdit::editingFinished,
                this, [this,productField]{ on_product_name_changed(productField);} );//baigus rasyt koda, rodomas pavadinimas

    connect(productFields.back()->productCodeInput, &QLineEdit::returnPressed,//nuspaudus 'enter' keiciam eilute
            this, [this, productField]{ setFocusOnAmountLineEdit(productField); } );
}
void MainWindow::addOptionButtons(){
    QGridLayout* buttonLayout = new QGridLayout();
    buttonLayout->addWidget(ui->addButton, 0, 0);
    buttonLayout->addWidget(ui->removeAllButton, 0, 1);
    buttonLayout->addWidget(ui->removeSelectedButton, 0, 2);

    for(int i = 0; i < 4; ++i){//padrom mazesnius mygtukus
        buttonLayout->addWidget(new QLabel(""), 0, 3 + i);
    }

    vWindowLayout->addLayout(buttonLayout);
}
void MainWindow::addSearchButton(){
    QGridLayout* buttonLayout = new QGridLayout();
    buttonLayout->addWidget(ui->search_button, 0, 0);

    for(int i = 0; i < 6; ++i){//padarom mygtukus mazesnius
        buttonLayout->addWidget(new QLabel(""), 0, 1 + i);
    }

    vWindowLayout->addLayout(buttonLayout);
}
void MainWindow::on_addButton_clicked()
{
    addProductFields(true);
}

void MainWindow::on_removeAllButton_clicked()
{
    productCodes.erase(productCodes.begin()+1, productCodes.end());
    productCodes[0] = "";

    auto itemAfterDefault = productBlockIndexes.find(productFields[1]);
    productBlockIndexes.erase(itemAfterDefault, productBlockIndexes.end());

    uint vectorSize = productFields.size() - 1;//paliekam defaultini lauka
    for(uint i = 0; i < vectorSize; ++i){
        delete productFields.back();
        productFields.pop_back();
    }
    productFields[0]->productCodeInput->setText("");//defaultini lauka isvalom
    productFields[0]->productNameOutput->setText("");//defaultini lauka isvalom

    ui->tableWidget->clear();
}
void MainWindow::on_removeSelectedButton_clicked()
{
    if(productFields.size() > 1){
        std::vector<int> removeIndexes;
        for(uint i = 1; i < productFields.size(); ++i){
            if(productFields[i]->checkBox->isChecked()){
                removeIndexes.push_back(i);
            }
        }

        for(uint index = 0; index < removeIndexes.size(); ++index){
            productCodes.erase(productCodes.begin() + removeIndexes[index] - index);
            delete productFields[removeIndexes[index] - index];
            productFields.erase(productFields.begin() + removeIndexes[index] - index);
        }
        on_search_button_clicked();
    }
}

void MainWindow::setMainWidgets(){
    addProductFields(false);
    addOptionButtons();
    addSearchButton();
    vWindowLayout->addWidget(ui->tableWidget);
}

void MainWindow::separateLines(int currentRow){
    for(int i = 0; i < ui->tableWidget->columnCount(); ++i){
        ui->tableWidget->setItem(currentRow, i, new QTableWidgetItem(""));
    }
}


