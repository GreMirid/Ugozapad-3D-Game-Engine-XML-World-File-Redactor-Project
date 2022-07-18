#include "redactor.h"
#include "ui_redactor.h"

Redactor::Redactor(QWidget *parent) : QMainWindow(parent), ui(new Ui::Redactor)
{
    ui->setupUi(this);
    it_not_choosen_file();
    setLocaledText();
}

Redactor::~Redactor()
{
    delete ui;
}

void Redactor::it_not_choosen_file()
{
    ui->file_text->setText(*m_local.getCurrentTextLocale("#warning_worldfile_main"));
    ui->file_geometry->setText(*m_local.getCurrentTextLocale("#warning_world_file"));
    ui->skybox_name->setText(*m_local.getCurrentTextLocale("#warning_world_file"));
    ui->cb_existense_entities->clear();
    ui->l_entities->clear();
    ui->l_args->clear();
    ui->le_value_of_parametr->clear();

    m_entities.clear();

    isFileLoaded = false;
}

void Redactor::displayEntityData(const QString &data)
{
    ui->l_entities->clear();
    ui->le_value_of_parametr->clear();

    for (int iter = 0; iter < m_entities.size(); iter++)
    {
        if (m_entities[iter].name == data)
        {
            ui->l_entities->addItem("section");
            ui->l_entities->addItem("name");
            for (int param = 0; param < m_entities[iter].params.size(); param++)
                ui->l_entities->addItem(m_entities[iter].params[param].valuename);
        }
    }
}

void Redactor::delete_entity(const QString &entityName)
{
    for (int iter = 0; iter < m_entities.size(); iter++)
        if (m_entities[iter].name == entityName)
            m_entities.removeAt(iter);
}

void Redactor::setLocaledText()
{
    //Menu Functions
    ui->menu->setTitle(*m_local.getCurrentTextLocale(ui->menu->title()));
    ui->action_2->setText(*m_local.getCurrentTextLocale(ui->action_2->text()));
    ui->action_about->setText(*m_local.getCurrentTextLocale(ui->action_about->text()));

    //Labels
    ui->file_text->setText(*m_local.getCurrentTextLocale(ui->file_text->text()));
    ui->label_4->setText(*m_local.getCurrentTextLocale(ui->label_4->text()));
    ui->label_5->setText(*m_local.getCurrentTextLocale(ui->label_5->text()));
    ui->label->setText(*m_local.getCurrentTextLocale(ui->label->text()));
    ui->label_2->setText(*m_local.getCurrentTextLocale(ui->label_2->text()));
    ui->label_6->setText(*m_local.getCurrentTextLocale(ui->label_6->text()));
    ui->label_3->setText(*m_local.getCurrentTextLocale(ui->label_3->text()));

    //Buttons
    ui->b_add_entity->setText(*m_local.getCurrentTextLocale(ui->b_add_entity->text()));
    ui->b_save_level->setText(*m_local.getCurrentTextLocale(ui->b_save_level->text()));
    ui->b_choose_file->setText(*m_local.getCurrentTextLocale(ui->b_choose_file->text()));
    ui->b_delete_entity->setText(*m_local.getCurrentTextLocale(ui->b_delete_entity->text()));
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
        while (!xml.atEnd() && !xml.hasError())
        {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (token == QXmlStreamReader::StartDocument) continue;
            if (token == QXmlStreamReader::StartElement)
            {
                if (xml.name() == "Level") { qDebug() << "LEVEL"; continue;}
                if (xml.name() == "Description") { qDebug() << "LEVEL DESCRIPTION"; continue;}
                if (xml.name() == "Entities") { qDebug() << "LEVEL ENTITIES"; continue;}
                if (xml.name() == "") continue;

                QXmlStreamAttributes attrib = xml.attributes();

                //level description zone
                if (xml.name() == "SceneFile")
                {
                    ui->file_geometry->setText(attrib.value("filename").toString());  qDebug() << "DAE GEOMETRY";
                }
                if (xml.name() == "Skybox")
                {
                    ui->skybox_name->setText(attrib.value("filename").toString()); qDebug() << "Skybox GEOMETRY";
                }

                //level entities zone
                if (xml.name() == "Entity")
                {
                    ui->cb_existense_entities->addItem(attrib.value("name").toString());
                    qDebug() << "ENTITY" << attrib.value("section").toString();

                    entity newEntity;

                    newEntity.name = attrib.value("name").toString();
                    newEntity.classname = attrib.value("section").toString();

                    xml.readNext();

                    while (!xml.hasError() && !(xml.name() == "Entity"))
                    {
                        QXmlStreamReader::TokenType token = xml.readNext();
                        QXmlStreamAttributes attrib = xml.attributes();
                        if (token == QXmlStreamReader::StartElement)
                        {
                            if (xml.name() == "CustomData") continue;

                            param newParam;
                            if (xml.name() != "")
                            {
                                newParam.valuename = xml.name().toString(); qDebug() << xml.name().toString();
                                for (int iter = 0; iter < attrib.size(); iter++)
                                {
                                    /*newParam.value += attrib[iter].name() + " = " + attrib[iter].value() + " ";*/
                                    argument argum = {attrib[iter].name().toString(), attrib[iter].value().toString()};

                                    qDebug() << argum.valuename << "="<< argum.value;

                                    newParam.argums.append(argum);
                                }
                            };
                            newEntity.params.append(newParam);
                        }
                    }
                    m_entities.append(newEntity);
                }
            }
            qDebug() << "next:" << xml.name() << "error:" << xml.hasError();
        }
        displayEntityData(ui->cb_existense_entities->currentText());
        isFileLoaded = true;
    }
    file->close();
}

void Redactor::on_b_choose_file_clicked()
{
    it_not_choosen_file(); // First clear all data in areas

    fileName = QFileDialog::getOpenFileName
            (this, *m_local.getCurrentTextLocale("#open_world_file"), "./../", tr("Ugozapad World File (*.xml)"));

    if (fileName == "") it_not_choosen_file(); else it_choosen_file();
}

void Redactor::on_action_about_triggered()
{
    QMessageBox::about
    (
         this,
         *m_local.getCurrentTextLocale("#about"),
         *m_local.getCurrentTextLocale("#about_expand_version") + " " +
         iniSettings->value("Main/ver", "").toString() + " " +
         *m_local.getCurrentTextLocale("#about_expand_version_comment") +
         *m_local.getCurrentTextLocale("#about_expand_body")
    );
}

void Redactor::on_cb_existense_entities_currentIndexChanged(const QString &arg1)
{
    displayEntityData(arg1);
}

#define CurParam m_entities[ui->cb_existense_entities->currentIndex()].params[currentRow - 2]

void Redactor::on_l_entities_currentRowChanged(int currentRow)
{
    qDebug() << "Current choosen row in entities is:" << currentRow;

    ui->l_args->clear();

    switch (currentRow)
    {
    case -1: break; //because it's clear and it's just crushing all program
    case 0:
        ui->le_value_of_parametr->setText(m_entities[ui->cb_existense_entities->currentIndex()].classname);
        break;
    case 1:
        ui->le_value_of_parametr->setText(m_entities[ui->cb_existense_entities->currentIndex()].name);
        break;
    default:

        if (CurParam.argums.size() == 1 && CurParam.argums[0].valuename == "value")
        {
            ui->le_value_of_parametr->setText(CurParam.argums[0].value);
        }
        else
        {
            for (int arg_id = 0; arg_id < CurParam.argums.size(); arg_id++)
                ui->l_args->addItem(CurParam.argums[arg_id].valuename);
            ui->le_value_of_parametr->setText(*m_local.getCurrentTextLocale("#choose_arg"));
        }
        break;
    }
}

#define CurArg m_entities[ui->cb_existense_entities->currentIndex()].params[ui->l_entities->currentRow() - 2]

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
    if ((arg1 != "" || arg1 != *m_local.getCurrentTextLocale("#choose_arg")) && isFileLoaded)
    {
        switch (ui->l_entities->currentRow())
        {
        case -1: break;
        case 0:
            m_entities[ui->cb_existense_entities->currentIndex()].classname = arg1;
            break;
        case 1:
            m_entities[ui->cb_existense_entities->currentIndex()].name = arg1;
            ui->cb_existense_entities->setItemText(ui->cb_existense_entities->currentIndex(), arg1);
            break;
        default:
            if (ui->l_args->currentRow() != -1)
                CurArg.argums[ui->l_args->currentRow()].value = arg1;
            else if (CurArg.argums[0].valuename == "value")
                CurArg.argums[0].value = arg1;
            break;
        }
    }
}

void Redactor::on_b_delete_entity_clicked()
{
    if (isFileLoaded)
    {
        delete_entity(ui->cb_existense_entities->currentText());
        ui->cb_existense_entities->removeItem(ui->cb_existense_entities->currentIndex());
        displayEntityData(ui->cb_existense_entities->currentText());
    }
}

void Redactor::on_b_add_entity_clicked()
{
    if (isFileLoaded)
    {

    }
}

void Redactor::on_b_save_level_clicked()
{
    if (isFileLoaded)
    {
        //XML Writing
        file->open(QIODevice::WriteOnly);

        QXmlStreamWriter xmlWriter(file);
        xmlWriter.setAutoFormatting(true);

        xmlWriter.writeStartDocument();

        //Let's start
        xmlWriter.writeStartElement("Level");

        //Descrition Level Section
        xmlWriter.writeStartElement("Description");

        xmlWriter.writeStartElement("SceneFile");
        xmlWriter.writeAttribute("filename", ui->file_geometry->text());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("Skybox");
        xmlWriter.writeAttribute("filename", ui->skybox_name->displayText());
        xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();

        //Entities Section
        xmlWriter.writeStartElement("Entities");

        for(int unit = 0; unit < m_entities.size(); unit++)
        {
            xmlWriter.writeStartElement("Entity");

            //Section, Name
            xmlWriter.writeAttribute("section", m_entities[unit].classname);
            xmlWriter.writeAttribute("name", m_entities[unit].name);

            //Transforms & Others
            for (int u_params = 0; u_params < m_entities[unit].params.size(); u_params++)
            {
                xmlWriter.writeStartElement(m_entities[unit].params[u_params].valuename);
                for (int u_args = 0; u_args < m_entities[unit].params[u_params].argums.size(); u_args++)
                {
                    xmlWriter.writeAttribute
                    (
                        m_entities[unit].params[u_params].argums[u_args].valuename,
                        m_entities[unit].params[u_params].argums[u_args].value
                    );
                }
                xmlWriter.writeEndElement();
            }

            //Custom Data
            xmlWriter.writeStartElement("CustomData");
            xmlWriter.writeEndElement();

            xmlWriter.writeEndElement();
        }

        xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();

        xmlWriter.writeEndDocument();
        file->close();

        // Close World
        it_not_choosen_file();
    }
}
