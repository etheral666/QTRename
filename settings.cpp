#include "settings.h"

// PRIVATE
//***********************************

bool Settings::validateFormat(QString newFormat, bool inpFormat)
{
    QRegExp regExp(":num\\d:");
    const QString forbiddenChars = ":\"\\/<>|?*";
    qint32 counter = 0, pos = 0;
    while(true)
    {
        pos = regExp.indexIn(newFormat, pos);
        if(pos++ == -1)
            break;
        ++counter;
    }
    if(counter == 1)
    {
        regExp.indexIn(newFormat);
        QString match = regExp.cap();
        qint32 numdigit = match[match.length() - 2].digitValue();
        if(!numdigit)
            return false;
        newFormat = newFormat.replace(regExp, "");

        if(!inpFormat)
        {
            newFormat = newFormat.replace(":teczka:", "");
            newFormat = newFormat.replace(":temat:", "");
        }
        for(int i = 0; i < forbiddenChars.length(); ++i)
        {
            if(newFormat.indexOf(forbiddenChars[i]) != -1)
                return false;
        }
    }
    return true;
}

bool Settings::validateDirectory(QString path)
{
    QDir testDir(path);
    if(testDir.exists())
        return true;
    else
        return false;
}

void Settings::setOutSuffix(const QString& subjectDir, const QString& fileDir)
{
    outputPathSuffix = "";

    if(checkBoxesSettings & Settings::CREATE_SUBJECT_DIRECTORIES)
        outputPathSuffix += ( (checkBoxesSettings & Settings::USE_MASKED_SUBJECT_DIRECTORIES) && maskedSubjectDirectories.contains(subjectDir.toInt()) ?
                                 maskedSubjectDirectories.value(subjectDir.toInt()) + "/" : subjectDir + "/");
    if(checkBoxesSettings & Settings::CREATE_FILE_DIRECTORIES)
        outputPathSuffix += fileDir + "/";
}

bool Settings::loadIniSettings()
{
    iniParser.beginGroup("Directory_settings");
    if(iniParser.value("customInputDirectory").toBool())
        checkBoxesSettings |= Settings::USE_CUSTOM_INPUT_FOLDER;
    if(iniParser.value("customOutputDirectory").toBool())
        checkBoxesSettings |= Settings::USE_CUSTOM_OUTPUT_FOLDER;
    inputDirectory = iniParser.value("inputDirectory").toString();
    outputDirectory = iniParser.value("outputDirectory").toString();
    iniParser.endGroup();

    iniParser.beginGroup("File_patterns");
    inputFormat = iniParser.value("inputFilePattern").toString();
    outputFormat = iniParser.value("outputFilePattern").toString();    
    inputNumFieldWidth = iniParser.value("inputNumFieldWidth").toInt();
    outputNumFieldWidth = iniParser.value("outputNumFieldWidth").toInt();
    iniParser.endGroup();

    iniParser.beginGroup("Copy_settings");
    if(iniParser.value("createSubjectDir").toBool())
        checkBoxesSettings |= Settings::CREATE_SUBJECT_DIRECTORIES;
    if(iniParser.value("createFileDir").toBool())
        checkBoxesSettings |= Settings::CREATE_FILE_DIRECTORIES;
    if(iniParser.value("deleteInputFiles").toBool())
        checkBoxesSettings |= Settings::DELETE_INPUT_FILES;
    if(iniParser.value("useMaskedSubjectDir").toBool())
        checkBoxesSettings |= Settings::USE_MASKED_SUBJECT_DIRECTORIES;
    SubWidth = iniParser.value("subWidth").toInt();
    FileWidth = iniParser.value("fileWidth").toInt();
    iniParser.endGroup();

    iniParser.beginReadArray("Masked_directories");
    QStringList keys = iniParser.allKeys();
    if(keys.size() > 0)
    {
        foreach(QString key, keys)
            maskedSubjectDirectories.insert(key.toInt(), iniParser.value(key).toString());
    }
    iniParser.endArray();

    return validateDirectory(inputDirectory) && validateFormat(inputFormat) && validateFormat(outputFormat, false);
}

// PUBLIC
//***********************************

Settings::Settings() :
    defaultDirectory(QCoreApplication::applicationDirPath() + "/Skany"),
    iniParser(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat)
{
    checkBoxesSettings = Settings::CLEAR_ALL_FLAGS;
    if( !QFile(QCoreApplication::applicationDirPath() + "/settings.ini").exists() || !loadIniSettings() )
    {
        loadDefaultSettings();
        saveSettingsToIniFile();
    }
}

void Settings::loadDefaultSettings()
{
    inputDirectory = defaultDirectory;
    outputDirectory = defaultDirectory;
    QDir defDir(defaultDirectory);
    if(!defDir.exists())
        defDir.mkdir(defaultDirectory);

    inputFormat = "Image:num4:.jpg";
    outputFormat = "F-:temat:-:teczka:-:num3:.jpg";
    inputNumFieldWidth = 4;
    outputNumFieldWidth = 3;

    checkBoxesSettings = Settings::CLEAR_ALL_FLAGS;
}

void Settings::saveSettingsToIniFile()
{
    iniParser.beginGroup("Directory_settings");
    iniParser.setValue("customInputDirectory", (bool)(checkBoxesSettings & Settings::USE_CUSTOM_INPUT_FOLDER));
    iniParser.setValue("customOutputDirectory", (bool)(checkBoxesSettings & Settings::USE_CUSTOM_OUTPUT_FOLDER));
    iniParser.setValue("inputDirectory", inputDirectory);
    iniParser.setValue("outputDirectory", outputDirectory);
    iniParser.endGroup();

    iniParser.beginGroup("File_patterns");
    iniParser.setValue("inputFilePattern", inputFormat);
    iniParser.setValue("outputFilePattern", outputFormat);
    iniParser.setValue("inputNumFieldWidth", inputNumFieldWidth);
    iniParser.setValue("outputNumFieldWidth", outputNumFieldWidth);
    iniParser.endGroup();

    iniParser.beginGroup("Copy_settings");
    iniParser.setValue("createSubjectDir", (bool)(checkBoxesSettings & Settings::CREATE_SUBJECT_DIRECTORIES));
    iniParser.setValue("createFileDir", (bool)(checkBoxesSettings & Settings::CREATE_FILE_DIRECTORIES));
    iniParser.setValue("deleteInputFiles", (bool)(checkBoxesSettings & Settings::DELETE_INPUT_FILES));
    iniParser.setValue("useMaskedSubjectDir", (bool)(checkBoxesSettings & Settings::USE_MASKED_SUBJECT_DIRECTORIES));
    iniParser.setValue("subWidth", SubWidth);
    iniParser.setValue("fileWidth", FileWidth);
    iniParser.endGroup();

    iniParser.beginGroup("Masked_directories");
    foreach(qint32 key, maskedSubjectDirectories.keys())
        iniParser.setValue(QString().setNum(key), maskedSubjectDirectories.value(key));
    iniParser.endGroup();
}

bool Settings::setInpDirectory(const QString &inputDir)
{
    if(!inputDir.isEmpty() && validateDirectory(inputDir))
    {
        inputDirectory = inputDir;
        return true;
    }
    else
        return false;
}

bool Settings::setOutDirectory(const QString &outputDir)
{
    if(!outputDir.isEmpty())
    {
        outputDirectory = outputDir;
        return true;
    }
    else
        return false;
}

bool Settings::setInputFormat(const QString &inpFormat)
{
    if(validateFormat(inpFormat))
    {
        inputFormat = inpFormat;
        QRegExp re(":num\\d:");
        re.indexIn(inpFormat);
        QString tmp = re.cap();
        inputNumFieldWidth = tmp[tmp.length() - 2].digitValue();
        return true;
    }
    else
        return false;
}

bool Settings::setOutputFormat(const QString &outFormat)
{
    if(validateFormat(outFormat, false))
    {
        outputFormat = outFormat;
        QRegExp re(":num\\d:");
        re.indexIn(outFormat);
        QString tmp = re.cap();
        outputNumFieldWidth = tmp[tmp.length() - 2].digitValue();
        return true;
    }
    else
        return false;
}

bool Settings::setMaskedDirectories(const QVector<qint32> &keys, const QStringList &directories)
{
    if(keys.count() != directories.count())
        return false;
    QMap<qint32, QString> tmp;
    const QString forbiddenChars = ":\"\\/<>|?*";
    for(int i = 0; i < keys.count(); ++i)
    {
        bool validName = true;
        for(int j = 0; j < forbiddenChars.count(); ++j)
            if(directories[i].indexOf(forbiddenChars[j]) != -1 || !directories[i][j].isLetterOrNumber())
            {
                validName = false;
                break;
            }
        if(validName)
            tmp.insert(keys[i], directories[i]);

    }
    if(tmp.isEmpty())
        return false;
    else
    {
        tmp.swap(maskedSubjectDirectories);
        return true;
    }
}

void Settings::setSubjectFileSubst(const QString &subject, const QString &file, qint32 subject_width, qint32 file_width)
{
    if(subject_width > 9 || subject_width < 1)
        SubWidth = 1;
    else
        SubWidth = subject_width;
    if(file_width > 9 || file_width < 1)
        FileWidth = 1;
    else
        FileWidth = file_width;

    subjectSubstitution = subject;
    while(subjectSubstitution.length() < SubWidth)
        subjectSubstitution = '0' + subjectSubstitution;
    fileSubstitution = file;
    while(fileSubstitution.length() < FileWidth)
        fileSubstitution = '0' + fileSubstitution;

    setOutSuffix(subjectSubstitution, fileSubstitution);
}
