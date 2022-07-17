#include "new_entity.h"
#include "ui_new_entity.h"

new_entity::new_entity(QWidget *parent) : QWidget(parent), ui(new Ui::new_entity)
{
    ui->setupUi(this);
}

new_entity::~new_entity()
{
    delete ui;
}
