#include "interactableFrame.h"
#include "ui_interactableFrame.h"

interactableFrame::interactableFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::interactableFrame)
{
    ui->setupUi(this);
}

interactableFrame::~interactableFrame()
{
    delete ui;
}
