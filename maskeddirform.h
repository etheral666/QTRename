#ifndef MASKEDDIRFORM_H
#define MASKEDDIRFORM_H

#include <QDialog>
#include "settings.h"

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
    void setSettingsPtr(Settings* sett) { _settings = sett; }

private slots:
    void on_addRowButton_clicked();
    void on_checkMasksButton_clicked();

    void on_finalizeButtonBox_accepted();

private:
    Ui::MaskedDirForm *ui;

    Settings* _settings;
    bool maskChecked;

    QList<qint32> subjectNums;
    QStringList subjectMasks;

    void checkMasks();
};

#endif // MASKEDDIRFORM_H
