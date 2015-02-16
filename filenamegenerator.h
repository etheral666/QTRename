#ifndef FILENAMEGENERATOR_H
#define FILENAMEGENERATOR_H

#include <QMap>
#include <QString>
#include <QRegExp>
#include <QFile>
#include "settings.h"

class FileNameGenerator
{
protected:
    Settings* _settings;

    QRegExp patternFinder;

public:
    FileNameGenerator(Settings* sett) : _settings(sett), patternFinder(":num\\d:") {}
    ~FileNameGenerator() {}

    QString generate(const QString &inpFile);

};

#endif // FILENAMEGENERATOR_H
