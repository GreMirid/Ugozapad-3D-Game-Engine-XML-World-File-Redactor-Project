#ifndef NEW_ENTITY_H
#define NEW_ENTITY_H

#include "pch.h"

namespace Ui {
class new_entity;
}

class new_entity : public QWidget
{
    Q_OBJECT

public:
    explicit new_entity(QWidget *parent = nullptr);
    ~new_entity();

private:
    Ui::new_entity *ui;
};

#endif // NEW_ENTITY_H
