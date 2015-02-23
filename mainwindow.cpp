#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mainSettings(new Settings()),
    maskedDirChanger(new MaskedDirForm),
    files(QMap<QString, QString>()),
    mover(mainSettings),
    manager(mainSettings, &files),
    fErrors(QFileDevice::NoError)
{
    this->setFixedSize(611, 416);
    maskedDirChanger->setSettingsPtr(mainSettings);
    sessionCounter = movedFilesCounter = 0;
    ui->setupUi(this);
    ui->statusBar->showMessage("Podczas bieżącej sesji przeniesiono lub skopiowano 0 plików.");
    updateSettingsDisplay();
}

MainWindow::~MainWindow()
{
    delete mainSettings;
    delete maskedDirChanger;
    delete ui;
}

void MainWindow::on_renameButton_clicked()
{
    ++sessionCounter;
    ui->logTextBrowser->append("Zamiana nr " + QString().setNum(sessionCounter) + " rozpoczęta.");
    mainSettings->setSubjectFileSubst(QString().setNum(ui->subjectSpinBox->value()),
                                      QString().setNum(ui->fileSpinBox->value()),
                                      ui->subjectWidthComboBox->currentIndex() + 1,
                                      ui->fileWidthComboBox->currentIndex() + 1);
    manager.getMatchingFiles();

    bool deleteInputFiles = mainSettings->getCheckBoxesFlags() & Settings::DELETE_INPUT_FILES;
    bool copyResult;
    if(files.empty())
        ui->logTextBrowser->append("Brak plików o podanym formacie w katalogu wejściowym.");
    else
    {
        if(!manager.createDir())
        {
            ui->logTextBrowser->setTextColor(Qt::red);
            ui->logTextBrowser->append("Nie udało się stworzyć katalogu na pliki wyjściowe, operacja przerwana.\n");
            ui->logTextBrowser->setTextColor(Qt::black);
            return;
        }
        foreach (QString file, files.keys())
        {
            copyResult = mover.copyFile(file, files.value(file), deleteInputFiles);
            if(copyResult)
            {
                ++movedFilesCounter;
                ui->logTextBrowser->setTextColor(Qt::darkGreen);
            }
            else
                ui->logTextBrowser->setTextColor(Qt::red);
            ui->logTextBrowser->append(file + " -> " + files.value(file));
            ui->logTextBrowser->setTextColor(Qt::black);
            if(!copyResult)
                ui->logTextBrowser->append(mover.getFileErrorDescription());
        }
    }
    ui->logTextBrowser->append("Zakończono.\n");
    ui->statusBar->showMessage("Podczas bieżącej sesji przeniesiono lub skopiowano " + QString().setNum(movedFilesCounter) + " plików.");
}

void MainWindow::on_inputDirCheckBox_clicked()
{
    if(ui->inputDirCheckBox->checkState() == Qt::Checked)
    {
        ui->inputDirectoryTextBrowser->setEnabled(true);
        ui->chooseInputDirButton->setEnabled(true);
    }
    else
    {
        ui->inputDirectoryTextBrowser->setEnabled(false);
        ui->chooseInputDirButton->setEnabled(false);
    }
}

void MainWindow::on_outputDirCheckBox_clicked()
{
    if(ui->outputDirCheckBox->checkState() == Qt::Checked)
    {
        ui->outputDirectoryTextBrowser->setEnabled(true);
        ui->chooseOutputDirButton->setEnabled(true);
    }
    else
    {
        ui->outputDirectoryTextBrowser->setEnabled(false);
        ui->chooseOutputDirButton->setEnabled(false);
    }
}

void MainWindow::on_chooseInputDirButton_clicked()
{
    QString tmp = QFileDialog::getExistingDirectory(this, "Wybierz katalog", ui->inputDirectoryTextBrowser->toPlainText(), QFileDialog::ShowDirsOnly);
    if(tmp.length())
        ui->inputDirectoryTextBrowser->setText(tmp);
}

void MainWindow::on_chooseOutputDirButton_clicked()
{
    QString tmp = QFileDialog::getExistingDirectory(this, "Wybierz katalog", ui->outputDirectoryTextBrowser->toPlainText(), QFileDialog::ShowDirsOnly);
    if(tmp.length())
        ui->outputDirectoryTextBrowser->setText(tmp);
}

void MainWindow::updateSettingsDisplay()
{
    //wyświetlenie katalogów w textBrowserach
    ui->inputDirectoryTextBrowser->setText(mainSettings->getInputDir());
    ui->outputDirectoryTextBrowser->setText(mainSettings->getOutputDir());

    //wyświeltenie formatów w textBrowserach
    ui->oldFilePatternTextEdit->setText(mainSettings->getInputFormat());
    ui->newFilePatternTextEdit->setText(mainSettings->getOutputFormat());

    //ustawienie checkBoxów
    int boxesFlags = mainSettings->getCheckBoxesFlags();
    bool tmp = boxesFlags & Settings::CREATE_SUBJECT_DIRECTORIES;
    ui->createSubfoldersCheckBox->setChecked(tmp);
    ui->createSubjectCheckBox->setChecked(tmp);
    ui->regularDirRadioButton->setEnabled(tmp);
    ui->maskedDirRadioButton->setEnabled(tmp);
    ui->showMasksButton->setEnabled(tmp);
    if(boxesFlags & Settings::USE_MASKED_SUBJECT_DIRECTORIES)
        ui->maskedDirRadioButton->setChecked(true);
    else
        ui->regularDirRadioButton->setChecked(true);
    ui->deleteInputFilesCheckBox->setChecked(boxesFlags & Settings::DELETE_INPUT_FILES);

    tmp = boxesFlags & Settings::USE_CUSTOM_INPUT_FOLDER;
    ui->inputDirCheckBox->setChecked(tmp);
    ui->inputDirectoryTextBrowser->setEnabled(tmp);
    ui->chooseInputDirButton->setEnabled(tmp);
    tmp = boxesFlags & Settings::USE_CUSTOM_OUTPUT_FOLDER;
    ui->outputDirCheckBox->setChecked(tmp);
    ui->outputDirectoryTextBrowser->setEnabled(tmp);
    ui->chooseOutputDirButton->setEnabled(tmp);

    //comboboxy na szerokość dodatkowych pól
    ui->subjectWidthComboBox->setCurrentIndex(mainSettings->getSubWidth() - 1);
    ui->fileWidthComboBox->setCurrentIndex(mainSettings->getFileWidth() - 1);

    //mapa masek
    maskedDirChanger->updateMaskedDirDisplay(mainSettings->getMaskedDirMap());
}

void MainWindow::on_defaultSettingsButton_clicked()
{
    QString msg = "Przywrócenie ustawień domyślnych spowoduje utratę obecnych ustawień (także masek), czy chcesz kontynuować?";
    if(QMessageBox::warning(this, "Ostrzeżenie", msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;
    mainSettings->loadDefaultSettings();
    updateSettingsDisplay();
}


void MainWindow::on_createSubjectCheckBox_clicked()
{
    if(ui->createSubjectCheckBox->checkState() == Qt::Checked)
    {
        ui->regularDirRadioButton->setEnabled(true);
        ui->maskedDirRadioButton->setEnabled(true);
        ui->showMasksButton->setEnabled(true);
    }
    else
    {
        ui->regularDirRadioButton->setEnabled(false);
        ui->maskedDirRadioButton->setEnabled(false);
        ui->showMasksButton->setEnabled(false);
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index != 1)
    {
        QString err = saveFormSettings();
        if(!err.isEmpty())
        {
            MsgBox.setText("Błąd podczas zapisu ustawień:\n" + err);
            MsgBox.exec();
            ui->tabWidget->setCurrentIndex(1);
            return;
        }
    }
}

QString MainWindow::saveFormSettings()
{
    QString errorMsg = "";
    int flags = 0;
    if(ui->createSubfoldersCheckBox->isChecked())
        flags |= Settings::CREATE_FILE_DIRECTORIES;
    if(ui->createSubjectCheckBox->isChecked())
    {
        flags |= Settings::CREATE_SUBJECT_DIRECTORIES;
        if(ui->maskedDirRadioButton->isChecked())
            flags |= Settings::USE_MASKED_SUBJECT_DIRECTORIES;
    }
    if(ui->deleteInputFilesCheckBox->isChecked())
        flags |= Settings::DELETE_INPUT_FILES;

    if(ui->inputDirCheckBox->isChecked())
    {
        flags |= Settings::USE_CUSTOM_INPUT_FOLDER;
        if(!mainSettings->setInpDirectory(ui->inputDirectoryTextBrowser->toPlainText()))
            errorMsg += " - nieprawidłowy katalog na pliki wejściowe;\n";
    }

    if(ui->outputDirCheckBox->isChecked())
    {
        flags |= Settings::USE_CUSTOM_OUTPUT_FOLDER;
        if(!mainSettings->setOutDirectory(ui->outputDirectoryTextBrowser->toPlainText()))
            errorMsg += " - nieprawidłowy katalog na pliki wyjściowe;\n";
    }

    mainSettings->setCheckBoxSettings((Settings::SETTINGS)flags);

    if(!mainSettings->setInputFormat(ui->oldFilePatternTextEdit->toPlainText()))
        errorMsg += " - nieprawidłowy format plików wejściowych;\n";
    if(!mainSettings->setOutputFormat(ui->newFilePatternTextEdit->toPlainText()))
        errorMsg += " - nieprawidłowy format plików wyjściowych;\n";

    return errorMsg;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mainSettings->saveSettingsToIniFile();
}

void MainWindow::on_showMasksButton_clicked()
{
    maskedDirChanger->exec();
}

void MainWindow::on_MainWindow_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction("O programie", this, SLOT(showContextInfo()));
    menu.exec(QCursor::pos());
}

void MainWindow::showContextInfo()
{
    QString txt = "<p align='center'>Oprogramowanie darmowe, do wewnętrznego użytku\n Archiwum Programowego OTV Wrocław.</p>"
                  "<p align='center'>Licencja GNU LGPL v3 i GNU LGPL v2.1</p>"
                  "<p align='center'>©Jakub Kulikowski 2015</p>";
    QMessageBox::information(this, "O programie", txt);
}
