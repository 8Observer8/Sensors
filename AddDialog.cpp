#include "AddDialog.h"
#include "ui_AddDialog.h"

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::on_addButton_clicked()
{
    QString name = ui->sensorNameLineEdit->text();
    int number = ui->portNumberSpinBox->text().toInt();
    emit signalNameAndNumber(name, number);
    close();
}

void AddDialog::on_cancelButton_clicked()
{
    close();
}
