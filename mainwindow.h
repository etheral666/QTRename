#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "filemover.h"
#include "filenamegenerator.h"

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

private:
    Ui::MainWindow *ui;
    QMessageBox MsgBox;

    QFile::FileError fErrors;

    FileMover mover;
    FileNameGenerator namesGenerator;

};

#endif // MAINWINDOW_H
