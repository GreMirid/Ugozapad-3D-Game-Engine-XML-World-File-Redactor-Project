#ifndef CORE_H
#define CORE_H

#include <QSettings>
#include <QString>
#include <QList>
#include <QMainWindow>
#include <QtXml>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QVariant>

#include <unordered_map>

//Main Defenitions
struct argument
{
    QString valuename, value;
};

struct param
{
    QString valuename;
    QList<argument> argums;
};

struct entity
{
    QString classname, name;
    QList<param> params;
};

extern QList<entity> m_entities;

//Settings from file
extern QSettings* iniSettings;

//Locals for Redactor
typedef QList<QPair<QString, QString*>>::iterator LangIter;

class LocaleManager
{
private:
    QString currentLang = "ru";

    QList<QPair<QString, QString*>> g_Locals;

public:
    LocaleManager();

    QString* getCurrentTextLocale(QString text);
};

extern LocaleManager m_local;

class Core
{
public:
    Core();
};

#endif // CORE_H
