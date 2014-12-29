#include "filenamegenerator.h"

FileNameGenerator::FileNameGenerator(QString& oldPattern, QString& newPattern)
    : patternFinder(":num\\d:")
{
    isOldPatternCorrect = setFilePattern(oldPattern, FileNameGenerator::OLD);
    isNewPatternCorrect = setFilePattern(newPattern, FileNameGenerator::NEW);
}

QString FileNameGenerator::generate(QString& oldFile)
{
    if(!patternsCorrect())
        return "";
    QString num = subjectSubstitution, result = newFilePattern;
    while(num.length() < outSubjWidth)
        num = '0' + num;
    result = result.replace(":temat:", num);
    num = fileSubstitution;
    while(num.length() < outFileWidth)
        num = '0' + num;
    result = result.replace(":teczka:", num);

    patternFinder.setPattern("\\d{" + QString().setNum(oldNumFieldWidth) + "}");
    patternFinder.indexIn(oldFile);
    num = patternFinder.cap();
    while(num.length() < newNumFieldWidth)
        num = '0' + num;
    if(num.length() > newNumFieldWidth)
        num = num.right(newNumFieldWidth);
    patternFinder.setPattern(":num\\d:");

    return result.replace(patternFinder, num);
}

bool FileNameGenerator::setFilePattern(const QString &pattern, FileNameGenerator::CHANGE_PATTERN oldOrNew)
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
            switch (oldOrNew)
            {
            case OLD:
                oldFilePattern = pattern;
                oldNumFieldWidth = numdigit;
                isOldPatternCorrect = true;
                break;
            case NEW:
                newFilePattern = pattern;
                newNumFieldWidth = numdigit;
                isNewPatternCorrect = true;
                break;
            default:
                 return isNewPatternCorrect = isOldPatternCorrect = false;
            }
            return true;
        }
    }
    isNewPatternCorrect = false;
    return false;
}

void FileNameGenerator::setSubjectAndFileWidth(quint32 sWidth, quint32 fWidth)
{
    if(sWidth && sWidth < 10)
        outSubjWidth = sWidth;
    else
        outSubjWidth = 1;
    if(fWidth && fWidth < 10)
        outFileWidth = fWidth;
    else
        outFileWidth = 1;
}
