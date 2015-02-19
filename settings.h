#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDir>
#include <QRegExp>
#include <QMap>
#include <QSettings>
#include <QtCore>

class Settings
{
private:

    const QString defaultDirectory;

    QSettings iniParser;

    QString inputDirectory;
    QString outputDirectory;
    QString outputPathSuffix;

    QString inputFormat;
    QString outputFormat;
    qint32 inputNumFieldWidth;
    qint32 outputNumFieldWidth;

    QString subjectSubstitution;
    QString fileSubstitution;
    qint32 SubWidth;
    qint32 FileWidth;

    qint32 checkBoxesSettings;

    QMap<qint32, QString> maskedSubjectDirectories;

    bool validateFormat(QString newFormat, bool inpFormat = true); //domyślnie sprawdza format wejściowy, false = wyjściowy
    bool validateDirectory(QString path);
    void setOutSuffix(const QString& subjectDir, const QString& fileDir); //wywoływana przez setSubjectFileSubst
    bool loadIniSettings();

public:
    enum SETTINGS
    {
        CLEAR_ALL_FLAGS = 0x0,
        USE_CUSTOM_INPUT_FOLDER = 0x1,
        USE_CUSTOM_OUTPUT_FOLDER = 0x2,
        CREATE_SUBJECT_DIRECTORIES = 0x4,
        CREATE_FILE_DIRECTORIES = 0x8,
        DELETE_INPUT_FILES = 0x10,
        USE_MASKED_SUBJECT_DIRECTORIES = 0x20
    };

    Settings();
    ~Settings() {}

    void loadDefaultSettings();
    void saveSettingsToIniFile(); //wywoływać dopiero po inicjalizacji pól (np. loadDefaultSettings())

    QString getInputDir() const {return inputDirectory;}
    QString getOutputDir() const {return outputDirectory;}
    QString getOutputSuffix() const {return outputPathSuffix;}
    QString getInputFormat() const {return inputFormat;}
    QString getOutputFormat() const {return outputFormat;}
    qint32 getInputNumFieldWidth() const {return inputNumFieldWidth;}
    qint32 getOutputNumFieldWidth() const {return outputNumFieldWidth;}
    qint32 getCheckBoxesFlags() const {return checkBoxesSettings;}
    QString getSubjSubst() const {return subjectSubstitution;}
    QString getFileSubst() const {return fileSubstitution;}
    qint32 getSubWidth() const {return SubWidth;}
    qint32 getFileWidth() const {return FileWidth;}
    QMap<qint32, QString>& getMaskedDirMap() {return maskedSubjectDirectories;}

    bool setInpDirectory(const QString& inputDir);
    bool setOutDirectory(const QString& outputDir);
    bool setInputFormat(const QString& inpFormat);
    bool setOutputFormat(const QString& outFormat);
    bool setMaskedDirectories(const QList<qint32>& keys, const QStringList& directories);
    void setCheckBoxSettings(SETTINGS flags) {checkBoxesSettings = flags;}
    void resetCheckBoxFlags(SETTINGS flags) {checkBoxesSettings &= !flags;}
    void setSubjectFileSubst(const QString& subject, const QString& file, qint32 subject_width, qint32 file_width);

};

#endif // SETTINGS_H
