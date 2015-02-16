#include "maskeddirform.h"
#include "ui_maskeddirform.h"

MaskedDirForm::MaskedDirForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaskedDirForm)
{
    ui->setupUi(this);
    setModal(true);
}

MaskedDirForm::~MaskedDirForm()
{
    delete ui;
}

void MaskedDirForm::updateMaskedDirDisplay(const QMap<qint32, QString> map)
{
    QList<qint32> keys = map.keys();
    qint32 row = 0;
    QTableWidgetItem *item;
    bool sorted = ui->maskedDirTableWidget->isSortingEnabled();
    ui->maskedDirTableWidget->setSortingEnabled(false);
    ui->maskedDirTableWidget->setRowCount(keys.count());
    foreach(qint32 key, keys)
    {
        item = new QTableWidgetItem(QString().setNum(key));
        ui->maskedDirTableWidget->setItem(row, 0, item);
        item = new QTableWidgetItem(map.value(key));
        ui->maskedDirTableWidget->setItem(row, 1, item);
        ++row;
    }
    ui->maskedDirTableWidget->setSortingEnabled(sorted);
}
