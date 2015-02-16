#ifndef FILEMOVER_H
#define FILEMOVER_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QMap>
#include "settings.h"

class FileMover
{
protected:    
    Settings* _settings;

    QFile currentFile;

public:
    explicit FileMover(Settings* sett) : _settings(sett) {}
    ~FileMover() {}

    QString getFileErrorDescription();

    bool copyFile(const QString& inputfilename, const QString& outputfilename, bool deleteinputfile = false);

};

#endif // FILEMOVER_H
