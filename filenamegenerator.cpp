#include "filenamegenerator.h"

FileNameGenerator::FileNameGenerator(QString &oldPattern, QString &newPattern)
    : patternFinder(":num\\d:")
{
    isOldPatternCorrect = setOldFilePattern(oldPattern);
    isNewPatternCorrect = setNewFilePattern(newPattern);
//    patternFinder.indexIn(oldFilePattern);
//    if(patternFinder.captureCount() == 1)
//    {
//        QString match = patternFinder.cap();
//        oldNumFieldWidth = match[match.length() - 2].digitValue();
//        if(oldNumFieldWidth)
//            isOldPatternCorrect = true;
//    }

//    patternFinder.indexIn(newFilePattern);
//    if(patternFinder.captureCount() == 1)
//    {
//        QString match = patternFinder.cap();
//        newNumFieldWidth = match[match.length() - 2].digitValue();
//        if(newNumFieldWidth)
//            isNewPatternCorrect = true;
//    }
}

QString FileNameGenerator::generate(QString& oldFile)//QString num)
{
    if(!patternsCorrect())
        return "";
    QString result = newFilePattern.replace(":temat:", subjectSubstitution);
    result = result.replace(":teczka:", fileSubstitution);

    patternFinder.setPattern("\\d{" + QString().setNum(oldNumFieldWidth) + "}");
    patternFinder.indexIn(oldFile);
    QString num = patternFinder.cap();
    while(num.length() < newNumFieldWidth)
        num = '0' + num;
    if(num.length() > newNumFieldWidth)
        num = num.right(newNumFieldWidth);
    patternFinder.setPattern(":num\\d:");

    return result.replace(patternFinder, num);

//    QString result = newFilePattern.replace(":temat:", subjectSubstitution);
//    result = result.replace(":teczka:", fileSubstitution);
//    while(num.length() < newNumFieldWidth)
//        num = '0' + num;
//    if(num.length() > newNumFieldWidth)
//        num = num.right(newNumFieldWidth);
//    QString rx = QString().setNum(newNumFieldWidth);
//    rx = "\\d{" + rx + "}";
//    patternFinder.setPattern(rx);
//    result = result.replace(patternFinder, num);
//    return result;
}

bool FileNameGenerator::setNewFilePattern(const QString &pattern)
{
    patternFinder.setPattern(":num\\d:");
    int counter = 0, pos = 0;
    while(true)
    {
        pos = patternFinder.indexIn(pattern, pos);
        if(pos++ == -1)
            break;
        ++counter;

    }
    if(counter == 1)
    {
        patternFinder.indexIn(pattern);
        QString match = patternFinder.cap();
        qint32 numdigit = match[match.length() - 2].digitValue();
        if(numdigit)
        {
            newFilePattern = pattern;
            newNumFieldWidth = numdigit;
            isNewPatternCorrect = true;
            return true;
        }
    }
    return false;
}

bool FileNameGenerator::setOldFilePattern(const QString &pattern)
{
    patternFinder.setPattern(":num\\d:");
    int counter = 0, pos = 0;
    while(true)
    {
        pos = patternFinder.indexIn(pattern, pos);
        if(pos++ == -1)
            break;
        ++counter;
    }
    if(counter == 1)
    {
        patternFinder.indexIn(pattern);
        QString match = patternFinder.cap();
        qint32 numdigit = match[match.length() - 2].digitValue();
        if(numdigit)
        {
            oldFilePattern = pattern;
            oldNumFieldWidth = numdigit;
            isOldPatternCorrect = true;
            return true;
        }
    }
    return false;
}
