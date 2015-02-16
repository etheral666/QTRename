#include "filesmanager.h"

FilesManager::FilesManager(Settings *sett, QMap<QString, QString> *map) :
    _settings(sett),
    filesPaths(map),
    namesGenerator(sett)
{

}

void FilesManager::getMatchingFiles()
{
    QStringList list = QDir(_settings->getInputDir()).entryList(QDir::Files, QDir::Name);
    if(list.empty())
        return;

    QRegExp re(":num\\d:");
    QString tmp = "\\d{" + QString().setNum(_settings->getInputNumFieldWidth()) + "}";
    tmp = _settings->getInputFormat().replace(re, tmp);
    re.setPattern(tmp);
    foreach (QString filename, list)
    {
        if(re.exactMatch(filename))
            filesPaths->insert(filename, namesGenerator.generate(filename));
    }
}

bool FilesManager::createDir(const QString &path)
{
    return QDir().mkpath(path);
}
