#include "filemover.h"

bool FileMover::copyFile(const QString &oldfilename, const QString &newfilename, bool deleteoldfile)
{
    QString old_path = oldFileDirectory.absolutePath() + "/";
    QString new_path = newFileDirectory.absolutePath() + "/";

    currentFile.setFileName(old_path + oldfilename);

    if(deleteoldfile)
        return currentFile.rename(new_path + newfilename);
    else
        return currentFile.copy(new_path + newfilename);
}

bool FileMover::copyFile(const QMap<QString, QString> &files, bool deleteoldfiles)
{
    bool result = true;
    QString old_path = oldFileDirectory.absolutePath() + "/";
    QString new_path = newFileDirectory.absolutePath() + "/";

    for(QMap<QString, QString>::const_iterator it = files.begin(); it != files.end() && result; ++it)
    {
        currentFile.setFileName(old_path + it.key());
        if(deleteoldfiles)
            result = currentFile.rename(new_path + it.value());
        else
            result = currentFile.copy(new_path + it.value());
    }
    return result;
}

bool FileMover::setOldFileDir(QString path)
{
    if(QDir(path).exists())
    {
        oldFileDirectory = QDir(path);
        return true;
    }
    return false;
}

bool FileMover::setNewFileDir(QString path)
{
    if(QDir(path).exists())
    {
        newFileDirectory = QDir(path);
        return true;
    }
    return false;
}

bool FileMover::operator()(const QString &oldfilename, const QString &newfilename, bool deleteoldfile)
{
    return copyFile(oldfilename, newfilename, deleteoldfile);
}
