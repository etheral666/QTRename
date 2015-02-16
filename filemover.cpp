#include "filemover.h"

QString FileMover::getFileErrorDescription()
{
    QString errDescription = "";
    switch(currentFile.error())
    {
    case QFileDevice::CopyError:
    case QFileDevice::RenameError:
        errDescription = "Błąd: plik istnieje lub brak praw dostępu do dysku."; break;
    default:
        errDescription = currentFile.errorString();
    }
    currentFile.unsetError();
    return errDescription;
}

bool FileMover::copyFile(const QString &inputfilename, const QString &outputfilename, bool deleteoldfile)
{
    QString inp_path = _settings->getInputDir() + "/";
    QString out_path = _settings->getOutputDir() + "/" + _settings->getOutputSuffix();

    currentFile.setFileName(inp_path + inputfilename);

    if(deleteoldfile)
        return currentFile.rename(out_path + outputfilename);
    else
        return currentFile.copy(out_path + outputfilename);
}
