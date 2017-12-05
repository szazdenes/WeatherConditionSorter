#include "sorterform.h"
#include "ui_sorterform.h"

SorterForm::SorterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SorterForm)
{
    ui->setupUi(this);
}

SorterForm::~SorterForm()
{
    delete ui;
}
