#ifndef FILENAMEGENERATOR_H
#define FILENAMEGENERATOR_H

#include <QMap>
#include <QString>
#include <QRegExp>

class FileNameGenerator
{
protected:
    QString oldFilePattern;
    QString newFilePattern;
    qint32 oldNumFieldWidth;
    qint32 newNumFieldWidth;
    bool isOldPatternCorrect;
    bool isNewPatternCorrect;

    QString subjectSubstitution; //wartość pola ":temat:"
    QString fileSubstitution; //wartość pola ":teczka:"

    QRegExp patternFinder;
public:
    FileNameGenerator() : isOldPatternCorrect(false), isNewPatternCorrect(false) {}
    FileNameGenerator(QString& oldPattern, QString& newPattern);
    ~FileNameGenerator() {}

    QString generate(QString& oldFile);

    void setSubjectSubstitution(const QString& subst) {subjectSubstitution = subst;}
    void setFileSubstitution(const QString& subst) {fileSubstitution = subst;}
    bool setNewFilePattern(const QString& pattern);
    bool setOldFilePattern(const QString& pattern);

    bool patternsCorrect() const {return isOldPatternCorrect && isNewPatternCorrect;}
};

#endif // FILENAMEGENERATOR_H
