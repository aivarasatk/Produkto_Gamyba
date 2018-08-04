#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <vector>
#include <map>
#include <string>

#include "productproduction.h"
#include "fileselection.h"
#include "productfields.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setHeaderItems(std::vector<QString> &headerItems);
    void insertItemToTable(int row, int &column, QString item);
    void putCodesToTable(CurrentProductData &containers);
    void displayListTableModel();

    void insertNumberValue(int row, int &column, double quantity);
    void showFileSelection();
    //
    void addCodeVariantsToTable(int row, int column, std::vector<QString> &codeVariants);
    QString formTableItem(const std::vector<QString> &codeVariants);

private slots:
    void on_search_button_clicked();
    void closeProgressBar();
    void on_addButton_clicked();

    void on_removeAllButton_clicked();

    void on_removeSelectedButton_clicked();

public slots:
    void on_product_name_changed(ProductFields* productField);//search for product name dynamically
    void setFocusOnAmountLineEdit(ProductFields* productField);
    void setWindowWidthToMatchTable();

private:
    Ui::MainWindow *ui;
    QVBoxLayout* vWindowLayout;

    std::vector<ProductFields*> productFields;
    std::map<ProductFields*, uint> productBlockIndexes;
    std::vector<QString> productCodes;

    ProductProduction pp;
    FileSelection *fileSelection;
    ProgressBar *progressBar;
    FileData fileData;
    std::vector<CurrentProductData> allProductData;

    void setHeaderNames(std::vector<QString>& headerItems);
    void setComponentColumns(CurrentProductData &containers, int currentRow);
    void setTableRowCount();
    void paintTable();

    void addProductFields(bool deletable);
    void addOptionButtons();
    void addSearchButton();

    void setMainWidgets();
    void separateLines(int currentRow);
};

#endif // MAINWINDOW_H
