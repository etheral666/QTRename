#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    mover.setNewFileDir(QString("D:/!focie"));
//    mover.setOldFileDir(QString("D:/!focie"));
//    ui->oldFilePatternTextEdit->setText("Image:num4:.jpg");
//    ui->newFilePatternTextEdit->setText(":num\\d:");

}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_pushButton_clicked()
//{
//    mover.copyFile(QString("Image0001.jpg"), QString("F-01-001-001.jpg"), false);
//    fErrors = mover.getFileError();
//    QString message;
//    switch (fErrors)
//    {
//    case QFileDevice::NoError:
//        message = "Operacja zakończona sukcesem.";
//        break;
//    case QFileDevice::RenameError:
//        message = "Nie udało się zmienić nazwy pliku.";
//        break;
//    case QFileDevice::CopyError:
//        message = "Nie udało się skopiować pliku.";
//        break;
//    case QFileDevice::RemoveError:
//        message = "Nie udało się usunąć pliku.";
//        break;
//    default:
//        message = "Wystąpił błąd:\n" + mover.getFileErrorDescription();
//        break;
//    }
//    MsgBox.setText(message);
//    MsgBox.exec();
//}

void MainWindow::on_renameButton_clicked()
{
    namesGenerator.setSubjectSubstitution(ui->subjectSpinBox->text());
    namesGenerator.setFileSubstitution(ui->fileSpinBox->text());

    QString txtEdit = QString().setNum(ui->startNumSpinBox->value());
    txtEdit = namesGenerator.generate(txtEdit);
    ui->logTextBrowser->append(txtEdit);
}

void MainWindow::on_saveSettingsButton_clicked()
{
    QString txtEdit = ui->oldFilePatternTextEdit->toPlainText();
    if(!namesGenerator.setOldFilePattern(txtEdit))
    {
        MsgBox.setText("Nieprawidłowy format szablonu plików wejściowych.");
        MsgBox.exec();
        return;
    }
    txtEdit = ui->newFilePatternTextEdit->toPlainText();
    if(!namesGenerator.setNewFilePattern(txtEdit))
    {
        MsgBox.setText("Nieprawidłowy format szablonu plików wyjściowych.");
        MsgBox.exec();
        return;
    }
}
