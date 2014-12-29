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
    quint32 outSubjWidth; //szerokość wyjściowa powyższych pól
    quint32 outFileWidth;

    QRegExp patternFinder;

public:

    enum CHANGE_PATTERN
    {
        OLD,
        NEW
    };

    FileNameGenerator() : isOldPatternCorrect(false), isNewPatternCorrect(false), patternFinder(":num\\d:") {}
    FileNameGenerator(const FileNameGenerator& gen) {*this = gen;}
    FileNameGenerator(QString& oldPattern, QString& newPattern);
    ~FileNameGenerator() {}

    QString generate(QString& oldFile);

    void setSubjectSubstitution(const QString& subst) {subjectSubstitution = subst;}
    void setFileSubstitution(const QString& subst) {fileSubstitution = subst;}
    bool setFilePattern(const QString& pattern, CHANGE_PATTERN oldOrNew);
    void setSubjectAndFileWidth(quint32 sWidth = 1, quint32 fWidth = 1);

    bool patternsCorrect() const {return isOldPatternCorrect && isNewPatternCorrect;}
};

#endif // FILENAMEGENERATOR_H
