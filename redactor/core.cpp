#include "core.h"

QList<entity> m_entities;

QSettings* iniSettings;

LocaleManager::LocaleManager()
{
    //Check .ini config
    iniSettings = new QSettings(QDir::currentPath() + "/conf.ini", QSettings::IniFormat);

    currentLang = iniSettings->value("Main/lang", "ru").toString();
    qDebug() << currentLang;
}

QString* LocaleManager::getCurrentTextLocale(QString text)
{
    QString* r_txt = new QString(text);

    QSettings iniLang(QDir::currentPath() + "/lang/" + currentLang + ".ini", QSettings::IniFormat);

    iniLang.setIniCodec("UTF-8");

    if (iniLang.value(currentLang + "/" + text, "").toString() == "") return r_txt;

    //Check
    for (LangIter it = g_Locals.begin(); it != g_Locals.end(); it++)
    {
        if (it->first == text)
            return it->second;
    }

    *r_txt = iniLang.value(currentLang + "/" + text, "").toString();

    //Insert
    QPair<QString, QString*> inside;
    inside.first = text;
    inside.second = r_txt;

    g_Locals.push_back(inside);

    //Return Adress
    return g_Locals.last().second;
}

LocaleManager m_local;

Core::Core()
{

}
