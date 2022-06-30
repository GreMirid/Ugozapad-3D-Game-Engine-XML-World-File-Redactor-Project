#ifndef REDACTOR_H
#define REDACTOR_H

#include <QMainWindow>
#include <QtXml>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Redactor; }
QT_END_NAMESPACE

class Redactor : public QMainWindow
{
    Q_OBJECT

public:
    Redactor(QWidget *parent = nullptr);
    ~Redactor();

private:
    QString fileName; //.xml file what we work for
    QFile* file;

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

    QList<entity> entities;

    void displayEntityData(const QString &data);

    void it_choosen_file();
    void it_not_choosen_file();

private slots:
    void on_b_choose_file_clicked();
    void on_action_about_triggered();
    void on_cb_existense_entities_currentIndexChanged(const QString &arg1);
    void on_l_entities_currentRowChanged(int currentRow);

    void on_l_args_currentRowChanged(int currentRow);

private:
    Ui::Redactor *ui;
};
#endif // REDACTOR_H
