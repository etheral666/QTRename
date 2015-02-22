#include "filenamegenerator.h"

QString FileNameGenerator::generate(const QString& inpFile)
{
    QString tmpPattern = _settings->getInputFormat();
    qint32 numFieldIdx = tmpPattern.indexOf(":");
    QString inpWidth = QString().setNum(_settings->getInputNumFieldWidth());

    tmpPattern = tmpPattern.replace(":num" + inpWidth + ":", "\\d{" + inpWidth + "}");
    patternFinder.setPattern(tmpPattern);
    if(!patternFinder.exactMatch(inpFile))
        return "";
    QString num = _settings->getSubjSubst(), result = _settings->getOutputFormat();
    qint32 width = _settings->getSubWidth();
    while(num.length() < width)
        num = '0' + num;
    result = result.replace(":temat:", num);
    num = _settings->getFileSubst();
    width = _settings->getFileWidth();
    while(num.length() < width)
        num = '0' + num;
    result = result.replace(":teczka:", num);
    patternFinder.setPattern("\\d{" + inpWidth + "}");
    patternFinder.indexIn(inpFile, numFieldIdx);
    num = patternFinder.cap();
    qint32 outWidth = _settings->getOutputNumFieldWidth();
    while(num.length() < outWidth)
        num = '0' + num;
    if(num.length() > outWidth)
        num = num.right(outWidth);
    patternFinder.setPattern(":num\\d:");

    return result.replace(patternFinder, num);
}
