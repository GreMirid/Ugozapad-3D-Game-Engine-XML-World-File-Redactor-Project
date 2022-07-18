#ifndef REDACTOR_H
#define REDACTOR_H

#include "new_entity.h"

#include "pch.h"

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

    bool isFileLoaded = false;

    void displayEntityData(const QString &data);

    void it_choosen_file();
    void it_not_choosen_file();
    void delete_entity(const QString &entityName); // from massive

    void setLocaledText();

private slots:
    void on_b_choose_file_clicked();
    void on_action_about_triggered();
    void on_cb_existense_entities_currentIndexChanged(const QString &arg1);
    void on_l_entities_currentRowChanged(int currentRow);
    void on_l_args_currentRowChanged(int currentRow);
    void on_le_value_of_parametr_textChanged(const QString &arg1);
    void on_b_delete_entity_clicked();
    void on_b_add_entity_clicked();

    void on_b_save_level_clicked();

private:
    Ui::Redactor *ui;
};
#endif // REDACTOR_H
