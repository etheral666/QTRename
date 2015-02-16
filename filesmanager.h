#ifndef FILESMANAGER_H
#define FILESMANAGER_H

#include <QMap>
#include <QString>
#include <QDir>
#include "filenamegenerator.h"
#include "settings.h"

class FilesManager
{
protected:
    Settings* _settings;
    QMap<QString, QString>* filesPaths;

    FileNameGenerator namesGenerator;

public:
    FilesManager(Settings* sett, QMap<QString, QString>* map);
    ~FilesManager() {}

    void getMatchingFiles();
    bool createDir(const QString& path);
};

#endif // FILESMANAGER_H
