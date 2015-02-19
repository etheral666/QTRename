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
    maskedDirChanger->updateMaskedDirDisplay(mainSettings->getMaskedDirMap());
    //ui->defaultSettingsButton->setToolTip("<div style= \"white-space: nowrap; width: 1500px;\">Some text for tooltip, which is too long</div>");
}

MainWindow::~MainWindow()
{
    delete mainSettings;
    delete maskedDirChanger;
    delete ui;
}

void MainWindow::on_renameButton_clicked()
{
    //todo
    //tutaj każdorazowo zmiana w filesmanager.generator -> zmiana parametrów bieżących
    //zasadniczo po kliknięciu:
    //1. uaktualnić filesmanager.generator
    //2. wygenerować mapę plików - w filesmanager będzie potrzebna funkcja
    //3. filemoverem wykorzystać mapę i przekopiować pliki, aktualizacja movera? raczej przy savesettings
    //4. wypisywać komunikaty na logTextBrowser

    //1.
    ++sessionCounter;
    ui->logTextBrowser->append("Zamiana nr " + QString().setNum(sessionCounter) + " rozpoczęta.");
    mainSettings->setSubjectFileSubst(QString().setNum(ui->subjectSpinBox->value()),
                                      QString().setNum(ui->fileSpinBox->value()),
                                      ui->subjectWidthComboBox->currentIndex() + 1,
                                      ui->fileWidthComboBox->currentIndex() + 1);
    if(!manager.createDir(mainSettings->getOutputDir() + "/" + mainSettings->getOutputSuffix()))
    {
        ui->logTextBrowser->setTextColor(Qt::red);
        ui->logTextBrowser->append("Nie udało się stworzyć katalogu na pliki wyjściowe, operacja przerwana.\n");
        ui->logTextBrowser->setTextColor(Qt::black);
        return;
    }
    //2.
    manager.getMatchingFiles();

    //3,4. testowe
    bool deleteInputFiles = mainSettings->getCheckBoxesFlags() & Settings::DELETE_INPUT_FILES;
    bool copyResult;
    if(files.empty())
        ui->logTextBrowser->append("Brak plików o podanym formacie w katalogu wejściowym.");
    else
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
    {
        ui->maskedDirRadioButton->setChecked(true);
    }
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

    ui->subjectWidthComboBox->setCurrentIndex(mainSettings->getSubWidth() - 1);
    ui->fileWidthComboBox->setCurrentIndex(mainSettings->getFileWidth() - 1);
}

void MainWindow::on_defaultSettingsButton_clicked()
{
    mainSettings->loadDefaultSettings();
    updateSettingsDisplay();
    maskedDirChanger->updateMaskedDirDisplay(mainSettings->getMaskedDirMap());
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
            MsgBox.setText(err);
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
            errorMsg += "Nieprawidłowy katalog na pliki wejściowe.\n";
    }

    if(ui->outputDirCheckBox->isChecked())
    {
        flags |= Settings::USE_CUSTOM_OUTPUT_FOLDER;
        if(!mainSettings->setOutDirectory(ui->outputDirectoryTextBrowser->toPlainText()))
            errorMsg += "Nieprawidłowy katalog na pliki wyjściowe.\n";
    }

    mainSettings->setCheckBoxSettings((Settings::SETTINGS)flags);
//    todo: po dorobieniu formularza dodać zapisywanie z QVector<qint32> i QStringList
//    if(!mainSettings->setMaskedDirectories())
//        errorMsg += "Nieprawidłe znaki w nazwach maskowanych katalogów.\n";

    if(!mainSettings->setInputFormat(ui->oldFilePatternTextEdit->toPlainText()))
        errorMsg += "Nieprawidłowy format plików wejściowych.\n";
    if(!mainSettings->setOutputFormat(ui->newFilePatternTextEdit->toPlainText()))
        errorMsg += "Nieprawidłowy format plików wyjściowych.\n";

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
