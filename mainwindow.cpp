#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_renameButton_clicked()
{
    namesGenerator.setSubjectSubstitution(ui->subjectSpinBox->text());
    namesGenerator.setFileSubstitution(ui->fileSpinBox->text());
    namesGenerator.setSubjectAndFileWidth(ui->subjectWidthComboBox->currentIndex() + 1,
                                          ui->fileWidthComboBox->currentIndex() + 1);

    QString txtEdit = ui->testTextEdit->toPlainText();
    txtEdit = namesGenerator.generate(txtEdit);
    ui->logTextBrowser->append(ui->testTextEdit->toPlainText() + " -> " + txtEdit);
}

void MainWindow::on_saveSettingsButton_clicked()
{
    QString txtEdit = ui->oldFilePatternTextEdit->toPlainText();
    if(!namesGenerator.setFilePattern(txtEdit, FileNameGenerator::OLD))
    {
        MsgBox.setText("Nieprawidłowy format szablonu plików wejściowych.");
        MsgBox.exec();
        return;
    }
    txtEdit = ui->newFilePatternTextEdit->toPlainText();
    if(!namesGenerator.setFilePattern(txtEdit, FileNameGenerator::NEW))
    {
        MsgBox.setText("Nieprawidłowy format szablonu plików wyjściowych.");
        MsgBox.exec();
        return;
    }
}
