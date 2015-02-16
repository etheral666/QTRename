#ifndef MASKEDDIRFORM_H
#define MASKEDDIRFORM_H

#include <QDialog>

namespace Ui {
class MaskedDirForm;
}

class MaskedDirForm : public QDialog
{
    Q_OBJECT

public:
    explicit MaskedDirForm(QWidget *parent = 0);
    ~MaskedDirForm();

    void updateMaskedDirDisplay(const QMap<qint32, QString> map);

private:
    Ui::MaskedDirForm *ui;
};

#endif // MASKEDDIRFORM_H
