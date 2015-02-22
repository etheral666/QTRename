#include "maskeddirform.h"
#include "ui_maskeddirform.h"

MaskedDirForm::MaskedDirForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaskedDirForm)
{
    ui->setupUi(this);
    setModal(true);
    this->setFixedSize(381, 481);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
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

void MaskedDirForm::on_addRowButton_clicked()
{
    ui->maskedDirTableWidget->setRowCount(ui->maskedDirTableWidget->rowCount() + 1);
    maskChecked = false;
}

void MaskedDirForm::on_checkMasksButton_clicked()
{
    checkMasks();
}

void MaskedDirForm::checkMasks()
{
    subjectMasks.clear();
    subjectNums.clear();
    bool ok;
    QTableWidgetItem* item;
    const QString forbiddenChars = ":\"\\/<>|?*";
    qint32 num;
    for(int i = 0; i < ui->maskedDirTableWidget->rowCount(); ++i)
    {
        item = ui->maskedDirTableWidget->item(i, 0);
        num = -1;
        if(item)
        {
            if(item->text().toInt(&ok, 10) < 0 || !ok)
                item->setTextColor(Qt::red);
            else
            {
                item->setTextColor(Qt::black);
                num = item->text().toInt(&ok, 10);
            }
        }
        item = ui->maskedDirTableWidget->item(i, 1);
        if(item)
        {
            ok = true;
            for(int j = 0; j < forbiddenChars.length(); ++j)
            {
                if(item->text().indexOf(forbiddenChars[j]) != -1)
                {
                    ok = false;
                    break;
                }
            }
            if(!ok)
                item->setTextColor(Qt::red);
            else
            {
                item->setTextColor(Qt::black);
                if(num != -1)
                {
                    subjectMasks.append(item->text());
                    subjectNums.append(num);
                }
            }
        }
    }
    maskChecked = true;
}

void MaskedDirForm::on_finalizeButtonBox_accepted()
{
    if(!maskChecked)
        checkMasks();
    _settings->setMaskedDirectories(subjectNums, subjectMasks);
}
