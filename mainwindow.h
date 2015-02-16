#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QMap>
#include <QFileDialog>
#include "filemover.h"
#include "filenamegenerator.h"
#include "filesmanager.h"
#include "settings.h"
#include "maskeddirform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_renameButton_clicked();
    void on_saveSettingsButton_clicked();    
    void on_defaultSettingsButton_clicked();
    void on_chooseInputDirButton_clicked();
    void on_chooseOutputDirButton_clicked();

    void on_inputDirCheckBox_clicked();
    void on_outputDirCheckBox_clicked();

    void on_createSubjectCheckBox_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_maskedDirRadioButton_toggled(bool checked);

    QString saveFormSettings();


    void on_showMasksButton_clicked();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    MaskedDirForm *maskedDirChanger;
    QMessageBox MsgBox;
    qint32 sessionCounter;
    qint32 movedFilesCounter;

    QMap<QString, QString> files;

    Settings *mainSettings;

    FileMover mover;
    FilesManager manager;

    QFile::FileError fErrors;

    void updateSettingsDisplay();
};

#endif // MAINWINDOW_H
