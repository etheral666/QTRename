#ifndef FILEMOVER_H
#define FILEMOVER_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QMap>

class FileMover
{
protected:

    QDir oldFileDirectory;
    QDir newFileDirectory;
    QFile currentFile;

public:
    FileMover() {}
    ~FileMover() {}

    QFileDevice::FileError getFileError() const {return currentFile.error();}
    QString getFileErrorDescription () const {return currentFile.errorString();}

    bool copyFile(const QString& oldfilename, const QString& newfilename, bool deleteoldfile = false);
    bool copyFile(const QMap<QString, QString> &files, bool deleteoldfiles = false);

    bool setOldFileDir(QString path);
    bool setNewFileDir(QString path);

    bool operator()(const QString& oldfilename, const QString& newfilename, bool deleteoldfile = false);
};

#endif // FILEMOVER_H
