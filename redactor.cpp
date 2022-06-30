#include "redactor.h"
#include "ui_redactor.h"

Redactor::Redactor(QWidget *parent) : QMainWindow(parent), ui(new Ui::Redactor)
{
    ui->setupUi(this);
}

Redactor::~Redactor()
{
    delete ui;
}

void Redactor::it_not_choosen_file()
{
    ui->file_text->setText("Выберите файл игрового мира Ugozapad (.xml)");
    ui->file_geometry->setText("Выберите файл игрового мира!");
    ui->skybox_name->setText("Выберите файл игрового мира!");
    ui->cb_existense_entities->clear();
    ui->l_entities->clear();
    ui->l_args->clear();
    ui->le_value_of_parametr->clear();

    entities.clear();
}

void Redactor::displayEntityData(const QString &data)
{
    ui->l_entities->clear();
    ui->le_value_of_parametr->clear();

    for (int iter = 0; iter < entities.size(); iter++)
    {
        if (entities[iter].name == data)
        {
            ui->l_entities->addItem("classname");
            ui->l_entities->addItem("name");
            for(int param = 0; param < entities[iter].params.size(); param++)
                ui->l_entities->addItem(entities[iter].params[param].valuename);
        }
    }
}

void Redactor::delete_entity(const QString &entityName)
{
    for (int iter = 0; iter < entities.size(); iter++)
    {
        if (entities[iter].name == entityName)
        {
            entities.removeAt(iter);
        }
    }
}

void Redactor::it_choosen_file()
{
    file = new QFile(fileName);

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) it_not_choosen_file();
    else
    {
        QXmlStreamReader xml(file);
        ui->file_text->setText(fileName);

        //Just read that
        while(!xml.atEnd() && !xml.hasError())
        {
            QXmlStreamReader::TokenType token = xml.readNext();
            if(token == QXmlStreamReader::StartDocument) continue;
            if(token == QXmlStreamReader::StartElement)
            {
                if(xml.name() == "Level") { qDebug() << "LEVEL"; continue;}
                if(xml.name() == "LevelDescription") { qDebug() << "LEVEL DESCRIPTION"; continue;}
                if(xml.name() == "Entities") { qDebug() << "LEVEL ENTITIES"; continue;}
                if(xml.name() == "") continue;

                QXmlStreamAttributes attrib = xml.attributes();

                //level description zone
                if(xml.name() == "SceneFile")
                {
                    ui->file_geometry->setText(attrib.value("filename").toString());  qDebug() << "DAE GEOMETRY";
                }
                if(xml.name() == "Skybox")
                {
                    ui->skybox_name->setText(attrib.value("name").toString()); qDebug() << "Skybox GEOMETRY";
                }

                //level entities zone
                if (xml.name() == "Entity")
                {
                    ui->cb_existense_entities->addItem(attrib.value("name").toString());
                    qDebug() << "ENTITY" << attrib.value("classname").toString();

                    entity newEntity;

                    newEntity.name = attrib.value("name").toString();
                    newEntity.classname = attrib.value("classname").toString();

                    xml.readNext();

                    while(!xml.hasError() && !(xml.name() == "Entity"))
                    {
                        QXmlStreamReader::TokenType token = xml.readNext();
                        QXmlStreamAttributes attrib = xml.attributes();
                        if(token == QXmlStreamReader::StartElement)
                        {
                            param newParam;
                            if(xml.name() != "")
                            {
                                newParam.valuename = xml.name().toString(); qDebug() << xml.name().toString();
                                for (int iter = 0; iter < attrib.size(); iter++)
                                {
                                    /*newParam.value += attrib[iter].name() + " = " + attrib[iter].value() + " ";*/
                                    argument argum = {attrib[iter].name().toString(), attrib[iter].value().toString()};
                                    newParam.argums.append(argum);
                                    qDebug() << argum.valuename << "="<< argum.value;
                                }
                            };
                            newEntity.params.append(newParam);
                        }
                    }
                    entities.append(newEntity);
                }
            }
            qDebug() << "next:" << xml.name() << "error:" << xml.hasError();
        }
        displayEntityData(ui->cb_existense_entities->currentText());
    }
}

void Redactor::on_b_choose_file_clicked()
{
    it_not_choosen_file(); // First clear all data in areas

    fileName = QFileDialog::getOpenFileName
            (this, tr("Открыть файл Игрового мира Ugozapad"), "./../", tr("Ugozapad World File (*.xml)"));

    if (fileName == "") it_not_choosen_file(); else it_choosen_file();
}

void Redactor::on_action_about_triggered()
{
    QMessageBox::about(this, "О Redactor", "ver 0.01 alpha\nСделан GreMirid\nДля модифицирования уровней игрого движка Ugozapad");
}

void Redactor::on_cb_existense_entities_currentIndexChanged(const QString &arg1)
{
    displayEntityData(arg1);
}

#define CurParam entities[ui->cb_existense_entities->currentIndex()].params[currentRow - 2]

void Redactor::on_l_entities_currentRowChanged(int currentRow)
{
    qDebug() << "Current choosen row in entities is:" << currentRow;

    ui->l_args->clear();

    switch (currentRow)
    {
    case -1: break; //because it's clear and it's just crushing all program
    case 0:
        ui->le_value_of_parametr->setText(entities[ui->cb_existense_entities->currentIndex()].classname);
        break;
    case 1:
        ui->le_value_of_parametr->setText(entities[ui->cb_existense_entities->currentIndex()].name);
        break;
    default:
        for(int arg_id = 0; arg_id < CurParam.argums.size(); arg_id++)
            ui->l_args->addItem(CurParam.argums[arg_id].valuename);
        ui->le_value_of_parametr->setText("Выберите Аргумент!");
        break;
    }
}

#define CurArg entities[ui->cb_existense_entities->currentIndex()].params[ui->l_entities->currentRow() - 2]

void Redactor::on_l_args_currentRowChanged(int currentRow)
{
    qDebug() << "Current choosen row in args is:" << currentRow;

    switch (currentRow)
    {
    case -1: break; //same
    default:
        ui->le_value_of_parametr->setText(CurArg.argums[currentRow].value);
        break;
    }
}

//Rewrite
void Redactor::on_le_value_of_parametr_textChanged(const QString &arg1)
{
    if (arg1 != "" || arg1 != "Выберите Аргумент!")
    {
        switch (ui->l_entities->currentRow())
        {
        case -1: break;
        case 0:
            entities[ui->cb_existense_entities->currentIndex()].classname = arg1;
            break;
        case 1:
            entities[ui->cb_existense_entities->currentIndex()].name = arg1;
            ui->cb_existense_entities->setItemText(ui->cb_existense_entities->currentIndex(), arg1);
            break;
        default:
            if (ui->l_args->currentRow() != -1)
                CurArg.argums[ui->l_args->currentRow()].value = arg1;
            break;
        }
    }
}

void Redactor::on_b_delete_entity_clicked()
{
    delete_entity(ui->cb_existense_entities->currentText());
    ui->cb_existense_entities->removeItem(ui->cb_existense_entities->currentIndex());
    displayEntityData(ui->cb_existense_entities->currentText());
}
