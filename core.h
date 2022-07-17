#ifndef CORE_H
#define CORE_H

#include <QString>
#include <QList>

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

class Core
{
public:
    Core();
};

#endif // CORE_H
